/* ====================================================================
 * Copyright (c) 2015.  All rights reserved.
 *
 * Author: xxx
 *
 * ====================================================================
 */

#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cstring>
#include <vector>
#include <algorithm>
#include <iostream>
#include <map>
#include <string>

#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/socket.h>

using namespace std;

/** \ingroup popt
 */
struct poptOption {
/*@observer@*/ /*@null@*/
    const char * longName;	/*!< may be NULL */
    char shortName;		/*!< may be NUL */
    int argInfo;
/*@shared@*/ /*@null@*/
    void * arg;			/*!< depends on argInfo */
    int val;			/*!< 0 means don't return, just update flag */
/*@observer@*/ /*@null@*/
    const char * descrip;	/*!< description for autohelp -- may be NULL */
/*@observer@*/ /*@null@*/
    const char * argDescrip;	/*!< argument description for autohelp */
};

/*@{*/
#define POPT_ARG_NONE		0	/*!< no arg */
#define POPT_ARG_STRING		1	/*!< arg will be saved as string */
#define POPT_ARG_INT		2	/*!< arg will be converted to int */
#define POPT_ARG_LONG		3	/*!< arg will be converted to long */
#define POPT_ARG_INCLUDE_TABLE	4	/*!< arg points to table */
#define POPT_ARG_CALLBACK	5	/*!< table-wide callback... must be
					   set first in table; arg points
					   to callback, descrip points to
					   callback data to pass */
#define POPT_ARG_INTL_DOMAIN    6       /*!< set the translation domain
					   for this table and any
					   included tables; arg points
					   to the domain string */
#define POPT_ARG_VAL		7	/*!< arg should take value val */
#define	POPT_ARG_FLOAT		8	/*!< arg will be converted to float */
#define	POPT_ARG_DOUBLE		9	/*!< arg will be converted to double */

#define POPT_ARG_MASK		0x0000FFFF
/*@}*/

enum {OPT_VERSION = 1000, OPT_DAEMON, OPT_SENDER, OPT_EXCLUDE, OPT_EXCLUDE_FROM,
      OPT_FILTER, OPT_COMPARE_DEST, OPT_COPY_DEST, OPT_LINK_DEST, OPT_HELP,
      OPT_INCLUDE, OPT_INCLUDE_FROM, OPT_MODIFY_WINDOW, OPT_MIN_SIZE, OPT_CHMOD,
      OPT_READ_BATCH, OPT_WRITE_BATCH, OPT_ONLY_WRITE_BATCH, OPT_MAX_SIZE,
      OPT_NO_D, OPT_APPEND, OPT_NO_ICONV,
      OPT_SERVER, OPT_REFUSED_BASE = 9000};

#define OLD_MAX_BLOCK_SIZE ((int32)1 << 29)

#define IOERR_GENERAL	(1<<0) /* For backward compatibility, this must == 1 */
#define IOERR_VANISHED	(1<<1)
#define IOERR_DEL_LIMIT (1<<2)

#define MAX_ARGS 1000
#define MAX_BASIS_DIRS 20
#define MAX_SERVER_ARGS (MAX_BASIS_DIRS*2 + 100)

#define MPLEX_BASE 7

#define NO_FILTERS	0
#define SERVER_FILTERS	1
#define ALL_FILTERS	2

#define XFLG_FATAL_ERRORS	(1<<0)
#define XFLG_OLD_PREFIXES	(1<<1)
#define XFLG_ANCHORED2ABS	(1<<2) /* leading slash indicates absolute */
#define XFLG_ABS_IF_SLASH	(1<<3) /* leading or interior slash is absolute */
#define XFLG_DIR2WILD3		(1<<4) /* dir/ match gets trailing *** added */

#define ATTRS_REPORT		(1<<0)
#define ATTRS_SKIP_MTIME	(1<<1)

#define FULL_FLUSH	1
#define NORMAL_FLUSH	0

#ifndef MAXPATHLEN
#define MAXPATHLEN 1024
#endif

int write_batch = 0;
int read_batch = 0;
int backup_dir_len = 0;
int backup_suffix_len;
unsigned int backup_dir_remainder;

char *backup_suffix = NULL;
char *tmpdir = NULL;
char *partial_dir = NULL;
char *basis_dir[MAX_BASIS_DIRS+1];
char *config_file = NULL;
char *shell_cmd = NULL;
char *logfile_name = NULL;
char *logfile_format = NULL;
char *stdout_format = NULL;
char *password_file = NULL;
char *rsync_path = NULL;
char *backup_dir = NULL;
char backup_dir_buf[MAXPATHLEN];
char *sockopts = NULL;
int rsync_port = 0;
int compare_dest = 0;
int copy_dest = 0;
int link_dest = 0;
int basis_dir_cnt = 0;
char *dest_option = NULL;
int verbose = 0;
int quiet = 0;
int output_motd = 1;
int log_before_transfer = 0;
int stdout_format_has_i = 0;
int stdout_format_has_o_or_i = 0;
int logfile_format_has_i = 0;
int logfile_format_has_o_or_i = 0;
int always_checksum = 0;
int list_only = 0;

int whole_file = -1;

int append_mode = 0;
int keep_dirlinks = 0;
int copy_dirlinks = 0;
int copy_links = 0;
int preserve_links = 0;
int preserve_hard_links = 0;
int preserve_acls = 0;
int preserve_xattrs = 0;
int preserve_perms = 0;
int preserve_executability = 0;
int preserve_devices = 0;
int preserve_specials = 0;
int preserve_uid = 0;
int preserve_gid = 0;
int preserve_times = 0;
int update_only = 0;
int cvs_exclude = 0;
int dry_run = 0;
int do_xfers = 1;
int ignore_times = 0;
int delete_mode = 0;
int delete_during = 0;
int delete_before = 0;
int delete_after = 0;
int delete_excluded = 0;
int remove_source_files = 0;
int one_file_system = 0;
int protocol_version;
int sparse_files = 0;
int do_compression = 0;
int def_compress_level;
int am_root = 0; /* 0 = normal, 1 = root, 2 = --super, -1 = --fake-super */
int am_server = 0;
int am_sender = 0;
int am_starting_up = 1;
int relative_paths = -1;
int implied_dirs = 1;
int numeric_ids = 0;
int allow_8bit_chars = 0;
int force_delete = 0;
int io_timeout = 0;
int prune_empty_dirs = 0;
int use_qsort = 0;
char *files_from = NULL;
int filesfrom_fd = -1;
char *filesfrom_host = NULL;
int eol_nulls = 0;
int protect_args = 0;
int human_readable = 0;
int recurse = 0;
int allow_inc_recurse = 1;
int xfer_dirs = -1;
int am_daemon = 0;
int do_stats = 0;
int do_progress = 0;
int connect_timeout = 0;
int keep_partial = 0;
int safe_symlinks = 0;
int copy_unsafe_links = 0;
int size_only = 0;
int daemon_bwlimit = 0;
int bwlimit = 0;
int fuzzy_basis = 0;
size_t bwlimit_writemax = 0;
int ignore_existing = 0;
int ignore_non_existing = 0;
int need_messages_from_generator = 0;
int max_delete = 0;
//OFF_T max_size = 0;
//OFF_T min_size = 0;
int ignore_errors = 0;
int modify_window = 0;
int blocking_io = -1;
int checksum_seed = 0;
int inplace = 0;
int delay_updates = 0;
long block_size = 0; /* "long" because popt can't set an int32. */
char *skip_compress = NULL;
static int daemon_opt;   /* sets am_daemon after option error-reporting */
static int omit_dir_times = 0;
static int F_option_cnt = 0;
static int modify_window_set;
static int itemize_changes = 0;
static int refused_delete, refused_archive_part, refused_compress;
static int refused_partial, refused_progress, refused_delete_before;
static int refused_delete_during;
static int refused_inplace, refused_no_iconv;
static char *max_size_arg, *min_size_arg;
static char tmp_partialdir[] = ".~tmp~";

int make_backups = 0;
char *batch_name = NULL;
char *iconv_opt = NULL;
char *default_af_hint = NULL;
char *bind_address = NULL;

static struct poptOption long_options[] = {
  /* longName, shortName, argInfo, argPtr, value, descrip, argDesc */
  {"help",             0,  POPT_ARG_NONE,   0, OPT_HELP, 0, 0 },
  {"version",          0,  POPT_ARG_NONE,   0, OPT_VERSION, 0, 0},
  {"verbose",         'v', POPT_ARG_NONE,   0, 'v', 0, 0 },
  {"no-verbose",       0,  POPT_ARG_VAL,    &verbose, 0, 0, 0 },
  {"no-v",             0,  POPT_ARG_VAL,    &verbose, 0, 0, 0 },
  {"quiet",           'q', POPT_ARG_NONE,   0, 'q', 0, 0 },
  {"motd",             0,  POPT_ARG_VAL,    &output_motd, 1, 0, 0 },
  {"no-motd",          0,  POPT_ARG_VAL,    &output_motd, 0, 0, 0 },
  {"stats",            0,  POPT_ARG_NONE,   &do_stats, 0, 0, 0 },
  {"human-readable",  'h', POPT_ARG_NONE,   0, 'h', 0, 0},
  {"no-human-readable",0,  POPT_ARG_VAL,    &human_readable, 0, 0, 0},
  {"no-h",             0,  POPT_ARG_VAL,    &human_readable, 0, 0, 0},
  {"dry-run",         'n', POPT_ARG_NONE,   &dry_run, 0, 0, 0 },
  {"archive",         'a', POPT_ARG_NONE,   0, 'a', 0, 0 },
  {"recursive",       'r', POPT_ARG_VAL,    &recurse, 2, 0, 0 },
  {"no-recursive",     0,  POPT_ARG_VAL,    &recurse, 0, 0, 0 },
  {"no-r",             0,  POPT_ARG_VAL,    &recurse, 0, 0, 0 },
  {"inc-recursive",    0,  POPT_ARG_VAL,    &allow_inc_recurse, 1, 0, 0 },
  {"no-inc-recursive", 0,  POPT_ARG_VAL,    &allow_inc_recurse, 0, 0, 0 },
  {"i-r",              0,  POPT_ARG_VAL,    &allow_inc_recurse, 1, 0, 0 },
  {"no-i-r",           0,  POPT_ARG_VAL,    &allow_inc_recurse, 0, 0, 0 },
  {"dirs",            'd', POPT_ARG_VAL,    &xfer_dirs, 2, 0, 0 },
  {"no-dirs",          0,  POPT_ARG_VAL,    &xfer_dirs, 0, 0, 0 },
  {"no-d",             0,  POPT_ARG_VAL,    &xfer_dirs, 0, 0, 0 },
  {"old-dirs",         0,  POPT_ARG_VAL,    &xfer_dirs, 4, 0, 0 },
  {"old-d",            0,  POPT_ARG_VAL,    &xfer_dirs, 4, 0, 0 },
  {"perms",           'p', POPT_ARG_VAL,    &preserve_perms, 1, 0, 0 },
  {"no-perms",         0,  POPT_ARG_VAL,    &preserve_perms, 0, 0, 0 },
  {"no-p",             0,  POPT_ARG_VAL,    &preserve_perms, 0, 0, 0 },
  {"executability",   'E', POPT_ARG_NONE,   &preserve_executability, 0, 0, 0 },
  {"acls",            'A', POPT_ARG_NONE,   0, 'A', 0, 0 },
  {"no-acls",          0,  POPT_ARG_VAL,    &preserve_acls, 0, 0, 0 },
  {"no-A",             0,  POPT_ARG_VAL,    &preserve_acls, 0, 0, 0 },
  {"xattrs",          'X', POPT_ARG_NONE,   0, 'X', 0, 0 },
  {"no-xattrs",        0,  POPT_ARG_VAL,    &preserve_xattrs, 0, 0, 0 },
  {"no-X",             0,  POPT_ARG_VAL,    &preserve_xattrs, 0, 0, 0 },
  {"times",           't', POPT_ARG_VAL,    &preserve_times, 1, 0, 0 },
  {"no-times",         0,  POPT_ARG_VAL,    &preserve_times, 0, 0, 0 },
  {"no-t",             0,  POPT_ARG_VAL,    &preserve_times, 0, 0, 0 },
  {"omit-dir-times",  'O', POPT_ARG_VAL,    &omit_dir_times, 1, 0, 0 },
  {"no-omit-dir-times",0,  POPT_ARG_VAL,    &omit_dir_times, 0, 0, 0 },
  {"no-O",             0,  POPT_ARG_VAL,    &omit_dir_times, 0, 0, 0 },
  {"modify-window",    0,  POPT_ARG_INT,    &modify_window, OPT_MODIFY_WINDOW, 0, 0 },
  {"super",            0,  POPT_ARG_VAL,    &am_root, 2, 0, 0 },
  {"no-super",         0,  POPT_ARG_VAL,    &am_root, 0, 0, 0 },
  {"fake-super",       0,  POPT_ARG_VAL,    &am_root, -1, 0, 0 },
  {"owner",           'o', POPT_ARG_VAL,    &preserve_uid, 1, 0, 0 },
  {"no-owner",         0,  POPT_ARG_VAL,    &preserve_uid, 0, 0, 0 },
  {"no-o",             0,  POPT_ARG_VAL,    &preserve_uid, 0, 0, 0 },
  {"group",           'g', POPT_ARG_VAL,    &preserve_gid, 1, 0, 0 },
  {"no-group",         0,  POPT_ARG_VAL,    &preserve_gid, 0, 0, 0 },
  {"no-g",             0,  POPT_ARG_VAL,    &preserve_gid, 0, 0, 0 },
  {0,                 'D', POPT_ARG_NONE,   0, 'D', 0, 0 },
  {"no-D",             0,  POPT_ARG_NONE,   0, OPT_NO_D, 0, 0 },
  {"devices",          0,  POPT_ARG_VAL,    &preserve_devices, 1, 0, 0 },
  {"no-devices",       0,  POPT_ARG_VAL,    &preserve_devices, 0, 0, 0 },
  {"specials",         0,  POPT_ARG_VAL,    &preserve_specials, 1, 0, 0 },
  {"no-specials",      0,  POPT_ARG_VAL,    &preserve_specials, 0, 0, 0 },
  {"links",           'l', POPT_ARG_VAL,    &preserve_links, 1, 0, 0 },
  {"no-links",         0,  POPT_ARG_VAL,    &preserve_links, 0, 0, 0 },
  {"no-l",             0,  POPT_ARG_VAL,    &preserve_links, 0, 0, 0 },
  {"copy-links",      'L', POPT_ARG_NONE,   &copy_links, 0, 0, 0 },
  {"copy-unsafe-links",0,  POPT_ARG_NONE,   &copy_unsafe_links, 0, 0, 0 },
  {"safe-links",       0,  POPT_ARG_NONE,   &safe_symlinks, 0, 0, 0 },
  {"copy-dirlinks",   'k', POPT_ARG_NONE,   &copy_dirlinks, 0, 0, 0 },
  {"keep-dirlinks",   'K', POPT_ARG_NONE,   &keep_dirlinks, 0, 0, 0 },
  {"hard-links",      'H', POPT_ARG_NONE,   0, 'H', 0, 0 },
  {"no-hard-links",    0,  POPT_ARG_VAL,    &preserve_hard_links, 0, 0, 0 },
  {"no-H",             0,  POPT_ARG_VAL,    &preserve_hard_links, 0, 0, 0 },
  {"relative",        'R', POPT_ARG_VAL,    &relative_paths, 1, 0, 0 },
  {"no-relative",      0,  POPT_ARG_VAL,    &relative_paths, 0, 0, 0 },
  {"no-R",             0,  POPT_ARG_VAL,    &relative_paths, 0, 0, 0 },
  {"implied-dirs",     0,  POPT_ARG_VAL,    &implied_dirs, 1, 0, 0 },
  {"no-implied-dirs",  0,  POPT_ARG_VAL,    &implied_dirs, 0, 0, 0 },
  {"i-d",              0,  POPT_ARG_VAL,    &implied_dirs, 1, 0, 0 },
  {"no-i-d",           0,  POPT_ARG_VAL,    &implied_dirs, 0, 0, 0 },
  {"chmod",            0,  POPT_ARG_STRING, 0, OPT_CHMOD, 0, 0 },
  {"ignore-times",    'I', POPT_ARG_NONE,   &ignore_times, 0, 0, 0 },
  {"size-only",        0,  POPT_ARG_NONE,   &size_only, 0, 0, 0 },
  {"one-file-system", 'x', POPT_ARG_NONE,   0, 'x', 0, 0 },
  {"no-one-file-system",0, POPT_ARG_VAL,    &one_file_system, 0, 0, 0 },
  {"no-x",             0,  POPT_ARG_VAL,    &one_file_system, 0, 0, 0 },
  {"update",          'u', POPT_ARG_NONE,   &update_only, 0, 0, 0 },
  {"existing",         0,  POPT_ARG_NONE,   &ignore_non_existing, 0, 0, 0 },
  {"ignore-non-existing",0,POPT_ARG_NONE,   &ignore_non_existing, 0, 0, 0 },
  {"ignore-existing",  0,  POPT_ARG_NONE,   &ignore_existing, 0, 0, 0 },
  {"max-size",         0,  POPT_ARG_STRING, &max_size_arg, OPT_MAX_SIZE, 0, 0 },
  {"min-size",         0,  POPT_ARG_STRING, &min_size_arg, OPT_MIN_SIZE, 0, 0 },
  {"sparse",          'S', POPT_ARG_VAL,    &sparse_files, 1, 0, 0 },
  {"no-sparse",        0,  POPT_ARG_VAL,    &sparse_files, 0, 0, 0 },
  {"no-S",             0,  POPT_ARG_VAL,    &sparse_files, 0, 0, 0 },
  {"inplace",          0,  POPT_ARG_VAL,    &inplace, 1, 0, 0 },
  {"no-inplace",       0,  POPT_ARG_VAL,    &inplace, 0, 0, 0 },
  {"append",           0,  POPT_ARG_NONE,   0, OPT_APPEND, 0, 0 },
  {"append-verify",    0,  POPT_ARG_VAL,    &append_mode, 2, 0, 0 },
  {"no-append",        0,  POPT_ARG_VAL,    &append_mode, 0, 0, 0 },
  {"del",              0,  POPT_ARG_NONE,   &delete_during, 0, 0, 0 },
  {"delete",           0,  POPT_ARG_NONE,   &delete_mode, 0, 0, 0 },
  {"delete-before",    0,  POPT_ARG_NONE,   &delete_before, 0, 0, 0 },
  {"delete-during",    0,  POPT_ARG_VAL,    &delete_during, 1, 0, 0 },
  {"delete-delay",     0,  POPT_ARG_VAL,    &delete_during, 2, 0, 0 },
  {"delete-after",     0,  POPT_ARG_NONE,   &delete_after, 0, 0, 0 },
  {"delete-excluded",  0,  POPT_ARG_NONE,   &delete_excluded, 0, 0, 0 },
  {"remove-sent-files",0,  POPT_ARG_VAL,    &remove_source_files, 2, 0, 0 }, /* deprecated */
  {"remove-source-files",0,POPT_ARG_VAL,    &remove_source_files, 1, 0, 0 },
  {"force",            0,  POPT_ARG_VAL,    &force_delete, 1, 0, 0 },
  {"no-force",         0,  POPT_ARG_VAL,    &force_delete, 0, 0, 0 },
  {"ignore-errors",    0,  POPT_ARG_VAL,    &ignore_errors, 1, 0, 0 },
  {"no-ignore-errors", 0,  POPT_ARG_VAL,    &ignore_errors, 0, 0, 0 },
  {"max-delete",       0,  POPT_ARG_INT,    &max_delete, 0, 0, 0 },
  {0,                 'F', POPT_ARG_NONE,   0, 'F', 0, 0 },
  {"filter",          'f', POPT_ARG_STRING, 0, OPT_FILTER, 0, 0 },
  {"exclude",          0,  POPT_ARG_STRING, 0, OPT_EXCLUDE, 0, 0 },
  {"include",          0,  POPT_ARG_STRING, 0, OPT_INCLUDE, 0, 0 },
  {"exclude-from",     0,  POPT_ARG_STRING, 0, OPT_EXCLUDE_FROM, 0, 0 },
  {"include-from",     0,  POPT_ARG_STRING, 0, OPT_INCLUDE_FROM, 0, 0 },
  {"cvs-exclude",     'C', POPT_ARG_NONE,   &cvs_exclude, 0, 0, 0 },
  {"whole-file",      'W', POPT_ARG_VAL,    &whole_file, 1, 0, 0 },
  {"no-whole-file",    0,  POPT_ARG_VAL,    &whole_file, 0, 0, 0 },
  {"no-W",             0,  POPT_ARG_VAL,    &whole_file, 0, 0, 0 },
  {"checksum",        'c', POPT_ARG_VAL,    &always_checksum, 1, 0, 0 },
  {"no-checksum",      0,  POPT_ARG_VAL,    &always_checksum, 0, 0, 0 },
  {"no-c",             0,  POPT_ARG_VAL,    &always_checksum, 0, 0, 0 },
  {"block-size",      'B', POPT_ARG_LONG,   &block_size, 0, 0, 0 },
  {"compare-dest",     0,  POPT_ARG_STRING, 0, OPT_COMPARE_DEST, 0, 0 },
  {"copy-dest",        0,  POPT_ARG_STRING, 0, OPT_COPY_DEST, 0, 0 },
  {"link-dest",        0,  POPT_ARG_STRING, 0, OPT_LINK_DEST, 0, 0 },
  {"fuzzy",           'y', POPT_ARG_VAL,    &fuzzy_basis, 1, 0, 0 },
  {"no-fuzzy",         0,  POPT_ARG_VAL,    &fuzzy_basis, 0, 0, 0 },
  {"no-y",             0,  POPT_ARG_VAL,    &fuzzy_basis, 0, 0, 0 },
  {"compress",        'z', POPT_ARG_NONE,   0, 'z', 0, 0 },
  {"no-compress",      0,  POPT_ARG_VAL,    &do_compression, 0, 0, 0 },
  {"no-z",             0,  POPT_ARG_VAL,    &do_compression, 0, 0, 0 },
  {"skip-compress",    0,  POPT_ARG_STRING, &skip_compress, 0, 0, 0 },
  {"compress-level",   0,  POPT_ARG_INT,    &def_compress_level, 'z', 0, 0 },
  {0,                 'P', POPT_ARG_NONE,   0, 'P', 0, 0 },
  {"progress",         0,  POPT_ARG_VAL,    &do_progress, 1, 0, 0 },
  {"no-progress",      0,  POPT_ARG_VAL,    &do_progress, 0, 0, 0 },
  {"partial",          0,  POPT_ARG_VAL,    &keep_partial, 1, 0, 0 },
  {"no-partial",       0,  POPT_ARG_VAL,    &keep_partial, 0, 0, 0 },
  {"partial-dir",      0,  POPT_ARG_STRING, &partial_dir, 0, 0, 0 },
  {"delay-updates",    0,  POPT_ARG_VAL,    &delay_updates, 1, 0, 0 },
  {"no-delay-updates", 0,  POPT_ARG_VAL,    &delay_updates, 0, 0, 0 },
  {"prune-empty-dirs",'m', POPT_ARG_VAL,    &prune_empty_dirs, 1, 0, 0 },
  {"no-prune-empty-dirs",0,POPT_ARG_VAL,    &prune_empty_dirs, 0, 0, 0 },
  {"no-m",             0,  POPT_ARG_VAL,    &prune_empty_dirs, 0, 0, 0 },
  {"log-file",         0,  POPT_ARG_STRING, &logfile_name, 0, 0, 0 },
  {"log-file-format",  0,  POPT_ARG_STRING, &logfile_format, 0, 0, 0 },
  {"out-format",       0,  POPT_ARG_STRING, &stdout_format, 0, 0, 0 },
  {"log-format",       0,  POPT_ARG_STRING, &stdout_format, 0, 0, 0 }, /* DEPRECATED */
  {"itemize-changes", 'i', POPT_ARG_NONE,   0, 'i', 0, 0 },
  {"no-itemize-changes",0, POPT_ARG_VAL,    &itemize_changes, 0, 0, 0 },
  {"no-i",             0,  POPT_ARG_VAL,    &itemize_changes, 0, 0, 0 },
  {"bwlimit",          0,  POPT_ARG_INT,    &bwlimit, 0, 0, 0 },
  {"no-bwlimit",       0,  POPT_ARG_VAL,    &bwlimit, 0, 0, 0 },
  {"backup",          'b', POPT_ARG_VAL,    &make_backups, 1, 0, 0 },
  {"no-backup",        0,  POPT_ARG_VAL,    &make_backups, 0, 0, 0 },
  {"backup-dir",       0,  POPT_ARG_STRING, &backup_dir, 0, 0, 0 },
  {"suffix",           0,  POPT_ARG_STRING, &backup_suffix, 0, 0, 0 },
  {"list-only",        0,  POPT_ARG_VAL,    &list_only, 2, 0, 0 },
  {"read-batch",       0,  POPT_ARG_STRING, &batch_name, OPT_READ_BATCH, 0, 0 },
  {"write-batch",      0,  POPT_ARG_STRING, &batch_name, OPT_WRITE_BATCH, 0, 0 },
  {"only-write-batch", 0,  POPT_ARG_STRING, &batch_name, OPT_ONLY_WRITE_BATCH, 0, 0 },
  {"files-from",       0,  POPT_ARG_STRING, &files_from, 0, 0, 0 },
  {"from0",           '0', POPT_ARG_VAL,    &eol_nulls, 1, 0, 0},
  {"no-from0",         0,  POPT_ARG_VAL,    &eol_nulls, 0, 0, 0},
  {"protect-args",    's', POPT_ARG_VAL,    &protect_args, 1, 0, 0},
  {"no-protect-args",  0,  POPT_ARG_VAL,    &protect_args, 0, 0, 0},
  {"no-s",             0,  POPT_ARG_VAL,    &protect_args, 0, 0, 0},
  {"numeric-ids",      0,  POPT_ARG_VAL,    &numeric_ids, 1, 0, 0 },
  {"no-numeric-ids",   0,  POPT_ARG_VAL,    &numeric_ids, 0, 0, 0 },
  {"timeout",          0,  POPT_ARG_INT,    &io_timeout, 0, 0, 0 },
  {"no-timeout",       0,  POPT_ARG_VAL,    &io_timeout, 0, 0, 0 },
  {"contimeout",       0,  POPT_ARG_INT,    &connect_timeout, 0, 0, 0 },
  {"no-contimeout",    0,  POPT_ARG_VAL,    &connect_timeout, 0, 0, 0 },
  {"rsh",             'e', POPT_ARG_STRING, &shell_cmd, 0, 0, 0 },
  {"rsync-path",       0,  POPT_ARG_STRING, &rsync_path, 0, 0, 0 },
  {"temp-dir",        'T', POPT_ARG_STRING, &tmpdir, 0, 0, 0 },
//#ifdef ICONV_OPTION
  {"iconv",            0,  POPT_ARG_STRING, &iconv_opt, 0, 0, 0 },
  {"no-iconv",         0,  POPT_ARG_NONE,   0, OPT_NO_ICONV, 0, 0 },
//#endif
  {"ipv4",            '4', POPT_ARG_VAL,    &default_af_hint, AF_INET, 0, 0 },
  {"ipv6",            '6', POPT_ARG_VAL,    &default_af_hint, AF_INET6, 0, 0 },
  {"8-bit-output",    '8', POPT_ARG_VAL,    &allow_8bit_chars, 1, 0, 0 },
  {"no-8-bit-output",  0,  POPT_ARG_VAL,    &allow_8bit_chars, 0, 0, 0 },
  {"no-8",             0,  POPT_ARG_VAL,    &allow_8bit_chars, 0, 0, 0 },
  {"qsort",            0,  POPT_ARG_NONE,   &use_qsort, 0, 0, 0 },
  {"address",          0,  POPT_ARG_STRING, &bind_address, 0, 0, 0 },
  {"port",             0,  POPT_ARG_INT,    &rsync_port, 0, 0, 0 },
  {"sockopts",         0,  POPT_ARG_STRING, &sockopts, 0, 0, 0 },
  {"password-file",    0,  POPT_ARG_STRING, &password_file, 0, 0, 0 },
  {"blocking-io",      0,  POPT_ARG_VAL,    &blocking_io, 1, 0, 0 },
  {"no-blocking-io",   0,  POPT_ARG_VAL,    &blocking_io, 0, 0, 0 },
  {"protocol",         0,  POPT_ARG_INT,    &protocol_version, 0, 0, 0 },
  {"checksum-seed",    0,  POPT_ARG_INT,    &checksum_seed, 0, 0, 0 },
  {"server",           0,  POPT_ARG_NONE,   0, OPT_SERVER, 0, 0 },
  {"sender",           0,  POPT_ARG_NONE,   0, OPT_SENDER, 0, 0 },
  /* All the following options switch us into daemon-mode option-parsing. */
  {"config",           0,  POPT_ARG_STRING, 0, OPT_DAEMON, 0, 0 },
  {"daemon",           0,  POPT_ARG_NONE,   0, OPT_DAEMON, 0, 0 },
  {"detach",           0,  POPT_ARG_NONE,   0, OPT_DAEMON, 0, 0 },
  {"no-detach",        0,  POPT_ARG_NONE,   0, OPT_DAEMON, 0, 0 },
  {0,0,0,0, 0, 0, 0}
};

int parse_rsync() {
  string sh_opts;

  struct poptOption *p = &long_options[0];
  for (size_t i = 0; i < sizeof(long_options)/sizeof(long_options[0]); ++i) {
    int has_arg = 1;
    if (p->argInfo == POPT_ARG_NONE ||
        p->argInfo == POPT_ARG_VAL ||
        p->argInfo == POPT_ARG_CALLBACK)
      has_arg = 0;

    if (p->longName) {
      printf("{\"%s\", %d, NULL, 0},\n", p->longName, has_arg);
    }

    if (p->shortName) {
      sh_opts.push_back(p->shortName);
      if (has_arg) {
        sh_opts.push_back(':');
      }
    }

    p++;
  }

  printf("short options: %s\n", sh_opts.c_str());

  return 0;
}
