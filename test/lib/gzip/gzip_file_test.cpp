/* ====================================================================
 * Copyright (c) 2015.  All rights reserved.
 *
 * Author: xxx
 *
 * ====================================================================
 */

#include "./gzip_file_test.h"


CPPUNIT_TEST_SUITE_REGISTRATION(gziptest);

gziptest::gziptest() {
}

gziptest::~gziptest() {
}

void gziptest::setUp() {
}

void gziptest::tearDown() {
}

int gzip_compress_file(const char* in, const char* out);

void gziptest::test1_gzip_compress_file() {
  const char* in = "../../../test/lib/gzip/file.in";
  const char* out = "../../../test/lib/gzip/file.gz";
  int result = gzip_compress_file(in, out);

  CPPUNIT_ASSERT(result == 0);
}

void gziptest::test2_gzip_compress_file() {
  const char* in = NULL;
  const char* out = "../../../test/lib/gzip/file.gz";
  int result = gzip_compress_file(in, out);

  CPPUNIT_ASSERT(result != 0);
}

void gziptest::test3_gzip_compress_file() {
  const char* in = "../../../test/lib/gzip/file.in";
  const char* out = NULL;
  int result = gzip_compress_file(in, out);

  CPPUNIT_ASSERT(result != 0);
}

