/* ====================================================================
 * Copyright (c) 2015.  All rights reserved.
 *
 * Author: xxx
 *
 * ====================================================================
 */

#include <cstdio>
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

int copy_file(const char *sourceFile, const char *targetFile) {
    FILE *fpR = nullptr, *fpW = nullptr;
    const int kBufSize = 4096;
    char buffer[kBufSize];
    size_t lenR, lenW;
    int ret = 0;

    do {
        if ((fpR = fopen(sourceFile, "r")) == nullptr) {
            ret = -1;
            break;
        }
        if ((fpW = fopen(targetFile, "w")) == nullptr) {
            ret = -1;
            break;
        }

        while ((lenR = fread(buffer, 1, kBufSize, fpR)) > 0) {
            if ((lenW = fwrite(buffer, 1, lenR, fpW)) != lenR) {
                ret = -1;
                break;
            }
        }

        if (ferror(fpR)) {
            ret = -1;
            break;
        }
    } while (0);

    if (fpR)
        fclose(fpR);

    if (fpW)
        fclose(fpW);

    return ret;
}
