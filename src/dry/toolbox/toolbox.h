/* ====================================================================
 * Copyright (c) 2015.  All rights reserved.
 *
 * Author: xxx
 *
 * ====================================================================
 */
#ifndef TOOLBOX_H
#define TOOLBOX_H

#include <string>

extern int read_file_to_str(const std::string &path, std::string &buf);
extern int copy_file(const char *sourceFile, const char *targetFile);

#endif /* TOOLBOX_H */

