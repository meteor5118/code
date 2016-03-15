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
#include <iostream>
#include <map>
#include <thread>
#include <atomic>
#include <string>
#include <algorithm>
#include <typeinfo>

#include <getopt.h>
#include <fcntl.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <signal.h>
#include <queue>
#include <pthread.h>

#include "backtrace.h"

using namespace std;

int main(int argc, char **argv){
    backtrace_ptrace(atoi(argv[1]), nullptr, nullptr, 0);

    return 0;
}

