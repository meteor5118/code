/* ====================================================================
 * Copyright (c) 2015.  All rights reserved.
 *
 * Author: xxx
 *
 * ====================================================================
 */

#include <string>
#include <fstream>

int read_file_to_str(const std::string &path, std::string &buf) {
    std::ifstream inFile(path);
    if (inFile) {
        buf.assign((std::istreambuf_iterator<char>(inFile)),
                      std::istreambuf_iterator<char>());
        return 0;
    }

    return 1;
}
