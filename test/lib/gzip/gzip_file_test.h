/* ====================================================================
 * Copyright (c) 2015.  All rights reserved.
 *
 * Author: xxx
 *
 * ====================================================================
 */

#ifndef GZIP_FILE_TEST_H
#define GZIP_FILE_TEST_H

#include <cppunit/extensions/HelperMacros.h>

class gziptest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(gziptest);

  CPPUNIT_TEST(test1_gzip_compress_file);
  CPPUNIT_TEST(test2_gzip_compress_file);
  CPPUNIT_TEST(test3_gzip_compress_file);

  CPPUNIT_TEST_SUITE_END();

 public:
  gziptest();
  virtual ~gziptest();
  void setUp();
  void tearDown();

 private:
  void test1_gzip_compress_file();
  void test2_gzip_compress_file();
  void test3_gzip_compress_file();
};

#endif /* GZIP_FILE_TEST_H */

