/* ====================================================================
 * Copyright (c) 2015.  All rights reserved.
 *
 * Author: xxx
 *
 * ====================================================================
 */

#include <cstdio>
#include <zlib.h>

/* Return value:
 *  0 success
 *  others failed
 */
int gzip_compress_file(const char *in, const char *out) {
  if (!in || !out)
    return Z_ERRNO;

  int err = Z_OK;
  unsigned char buf[4096];
  int read_len, write_len;
  FILE* fpIn = fopen(in, "rb");
  gzFile fpOut = gzopen(out, "wb9");

  if (fpIn != NULL && fpOut != NULL) {
      while (!feof(fpIn)) {
          read_len = fread(buf, 1, sizeof(buf), fpIn);
          if (ferror(fpIn)) {
              err = Z_ERRNO;
              break;
          }

          write_len = gzwrite(fpOut, buf, read_len);
          if (write_len != read_len) {
              gzerror(fpOut, &err);
              break;
          }
      }
  } else {
      err = Z_ERRNO;
  }

  if (fpIn) fclose(fpIn);
  if (fpOut) gzclose(fpOut);

  return err;
}
