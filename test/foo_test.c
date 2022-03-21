#include <check.h>

#include "../src/foo.h"
#include "libccg_test.h"

START_TEST(foo_func) { ck_assert_int_eq(func(), 0); }
END_TEST

Suite *foo_suite() {
  Suite *s;
  TCase *tc;

  s = suite_create("Foo");
  tc = tcase_create("Core");

  tcase_add_test(tc, foo_func);

  suite_add_tcase(s, tc);
  return s;
}