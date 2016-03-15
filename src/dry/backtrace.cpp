/*
 * All rights reserved.
 */
#include <libunwind-ptrace.h>
#include <libunwind.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/ptrace.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cctype>

#define SLEEP_WAIT 500
//extern unw_accessors_t snapshot_addr_space_accessors;
static int sleep_time = 0;

static int opt_show_rsp = 1;
static int opt_verbose = 1;
static int sleep_count = 0;
static int opt_use_waitpid_timeout = 0;
static int stop_timeout = 1000000;

static int detach_process(int pid)
{
    int rc = 0;
    if (ptrace(PTRACE_DETACH, pid, NULL, NULL) < 0) {
        perror("detach");
        rc = -1;
    }
    if (kill(pid, SIGCONT) < 0) {
        perror("send SIGCONT");
        rc = -1;
    }

//    attached_pid = 0;
//    gettimeofday(&unfreeze_time, NULL);
    return rc;
}

int proc_stopped(int pid)
{
    FILE *f;
    char buf[128];
    char c;
    int rc = -1;

    sprintf(buf, "/proc/%d/status", pid);
    if ((f = fopen(buf, "r")) == NULL) {
        fprintf(stderr, "cannot open %s: %s\n", buf, strerror(errno));
        return -1;
    }

    while (fgets(buf, sizeof(buf), f)) {
        if (sscanf(buf, "State:\t%c", &c) == 1) {
            rc = (c == 't' || c == 'T');
            break;
        }
    }

    fclose(f);
    return rc;
}

int attach_process(int pid)
{
    int status = 0;

//    gettimeofday(&freeze_time, NULL);

//    attached_pid = pid;
    if (ptrace(PTRACE_ATTACH, pid, NULL, NULL) < 0) {
        perror("attach");
        detach_process(pid);
        return -1;
    }
    if (!proc_stopped(pid)) {
        struct itimerval tm;

        if (opt_use_waitpid_timeout) {
            /* setup alarm to avoid long waiting on waitpid */
            tm.it_interval.tv_sec = 0;
            tm.it_interval.tv_usec = 0;
            tm.it_value.tv_sec = 1;
            tm.it_value.tv_usec = stop_timeout % 1000000;
            setitimer(ITIMER_REAL, &tm, NULL);
        }

        if (waitpid(pid, &status, WUNTRACED) < 0) {
            if (errno == EINTR) {
                fprintf(stderr, "timeout on waitpid\n");
                detach_process(pid);
                return -1;
            }
            fprintf(stderr, "waitpid %d: %s\n", pid, strerror(errno));
            detach_process(pid);
            return -1;
        }

        if (opt_use_waitpid_timeout) {
            tm.it_value.tv_sec = 0;
            tm.it_value.tv_usec = 0;
            setitimer(ITIMER_REAL, &tm, NULL);
        }

        if (!WIFSTOPPED(status) || WSTOPSIG(status) != SIGSTOP)
            fprintf(stderr, "warning: waitpid(%d) WIFSTOPPED=%d WSTOPSIG=%d\n",
                    pid, WIFSTOPPED(status), WSTOPSIG(status));
    }
//    stopped_pid = pid;
    if (kill(pid, SIGSTOP) < 0) {
        perror("send SIGSTOP");
        return -1;
    }
    return 0;
}

int wait_thread(int tid)
{
    int rc;
    while (!(rc = proc_stopped(tid))) {
        if (stop_timeout && sleep_time > stop_timeout) {
            fprintf(stderr, "timeout waiting for thread %d to stop", tid);
            return -1;
        }
        usleep(SLEEP_WAIT);
        sleep_time += SLEEP_WAIT;
        sleep_count++;
    }
    return (rc == -1 ? -1 : 0);
}

int attach_thread(int tid)
{
//    attached_tid = tid;
    if (ptrace(PTRACE_ATTACH, tid, NULL, NULL) < 0) {
        perror("PTRACE_ATTACH");
        return -1;
    }
    if (wait_thread(tid) < 0)
        return -1;
    return 0;
}

int adjust_threads(int *tids, int nr_tids, int *user_tids,
        int *index, int nr_user)
{
    int i, j, n = 0;
    for (i = 0; i < nr_user; ++i) {
        int found = 0;
        for (j = 0; j < nr_tids; ++j) {
            if (tids[j] == user_tids[i]) {
                found = 1;
                break;
            }
        }
        if (!found) {
            if (n || (user_tids[i] > nr_tids) || (user_tids[i] <= 0)) {
                fprintf(stderr, "unexpected thread %d\n", user_tids[i]);
                return -1;
            }
        } else {
            ++n;
            index[i] = j + 1;
        }
    }
    if (!n) {
        for (i = 0; i < nr_user; ++i) {
            index[i] = user_tids[i];
            user_tids[i] = tids[user_tids[i]-1];
        }
    }
    return 0;
}

int detach_thread(int tid)
{
    long rc = ptrace(PTRACE_DETACH, tid, NULL, NULL);
//    attached_tid = 0;
    if (rc < 0) {
        perror("PTRACE_DETACH");
        return -1;
    }
    return 0;
}

static int dir_select(const struct dirent *entry)
{
    const char *c = entry->d_name;
    while (*c)
        if (!isdigit(*c++))
            return 0;
    return 1;
}

int get_threads(int pid, int **tids)
{
    char buf[32];
    struct dirent **namelist;
    int cur, i, n;

    snprintf(buf, sizeof(buf), "/proc/%d/task", pid);

    n = scandir(buf, &namelist, dir_select, NULL);
    if (n < 0) {
        perror(buf);
        return -1;
    } else {
        *tids = reinterpret_cast<int *>(malloc(sizeof(int)*n));
        i = 0;
        while (i < n) {
            cur = atoi(namelist[i]->d_name);
            (*tids)[i] = cur;
            free(namelist[i++]);
        }
        free(namelist);
    }

    return n;
}

static int backtrace_thread(unw_accessors_t *accessors, void *arg)
{
    unw_addr_space_t addr_space;
    unw_cursor_t cursor;
    int rc = 0, n = 0;

    if ((addr_space = unw_create_addr_space(accessors, 0)) == NULL) {
        fprintf(stderr, "failed to create address space for unwinding\n");
        return -1;
    }

    if ((rc = unw_init_remote(&cursor, addr_space, arg)) < 0) {
        fprintf(stderr, "failed to init cursor for unwinding: rc=%d\n", rc);
        return -1;
    }

    do {
        unw_word_t ip, sp = -1, off;
        static char buf[512];
        size_t len;

        if ((rc = unw_get_reg(&cursor, UNW_REG_IP, &ip)) < 0) {
            fprintf(stderr, "failed to get IP: rc=%d\n", rc);
            break;
        }

        buf[0] = '\0';
        unw_get_proc_name(&cursor, buf, sizeof(buf), &off);

        if (buf[0] == '\0') {
            buf[0] = '?';
            buf[1] = '\0';
            len = 1;
        } else {
            len = strlen(buf);
        }

        if (len >= sizeof(buf) - 32)
            len = sizeof(buf) - 32;

        if (!ip)
            break;

        if (off) {
            sprintf(buf + len, " + 0x%lx", (unsigned long)off);
        }
        if (!opt_show_rsp) {
            printf(" %016lx  %s\n", (long)ip, buf);
        } else {
            unw_get_reg(&cursor, UNW_REG_SP, &sp);
            printf(" %016lx  %016lx  %s\n", (long)ip, (long)sp, buf);
        }

        if ((rc = unw_step(&cursor)) < 0) {
            if (!opt_show_rsp)
                printf(" ????????????????  <stack breaks here>\n");
            else
                printf(" ????????????????  ????????????????  <stack breaks here>\n");

            if (opt_verbose) {
                fprintf(stderr, "unwind step failed: n=%d rc=%d\n", n, rc);
            }
            break;
        }

        if (++n == 64 && rc) {
            puts(" ????????????????  <stack is too long>\n");
            break;
        }
    } while (rc > 0);

    unw_destroy_addr_space(addr_space);

    return rc;
}

int backtrace_ptrace(int pid, int *tids, int *index, int nr_tids)
{
#if !defined (NO_LIBUNWIND_PTRACE)
    int i, count, rc = 0;
    int *threads = NULL;

    count = get_threads(pid, &threads);
    if (!count || threads == NULL)
        return -1;

    if (tids != NULL) {
        if (adjust_threads(threads, count, tids, index, nr_tids) < 0)
            return -1;

        free(threads);
        count = nr_tids;
        threads = tids;
    }

    if (attach_process(pid) < 0)
        return -1;

    for (i = 0; i < count; ++i) {
        void *upt_info;

        printf("--------------------  thread %d (%d)  --------------------\n",
               (index != NULL ? index[i] : i+1), threads[i]);

        if (threads[i] != pid && attach_thread(threads[i]) < 0) {
            rc = -1;
            break;
        }

        upt_info = _UPT_create(threads[i]);

        if (backtrace_thread(&_UPT_accessors, upt_info) < 0)
            rc = -1;

        _UPT_destroy(upt_info);

        if (threads[i] != pid && detach_thread(threads[i]))
            rc = -1;
        if (rc < 0)
            break;
    }

    free(threads);

    if (detach_process(pid) < 0)
        return -1;

    return rc;

#else
    return -1;
#endif /* NO_LIBUNWIND_PTRACE */
}
