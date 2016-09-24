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

#include <iostream>
#include <vector>
#include <map>
#include <thread>
#include <atomic>
#include <string>
#include <list>
#include <set>
#include <memory>
#include <typeinfo>
#include <iterator>
#include <fstream>
#include <queue>
#include <algorithm>
#include <numeric>
#include <typeinfo>
#include <getopt.h>

#ifdef __linux__
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <signal.h>
#include <pthread.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <arpa/inet.h>

#endif

#include "toolbox/toolbox.h"

int main(int argc, char **argv){
    std::cout << copy_file("in", "out") << std::endl;
    return 0;
}

