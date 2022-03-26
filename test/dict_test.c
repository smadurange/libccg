
#include <check.h>

#include "../src/dict.h"
#include "../src/mem.h"
#include "libccg_test.h"

Suite *dict_suite() {
  Suite *s;
  TCase *tc;

  s = suite_create("dictionary");
  tc = tcase_create("core");

  suite_add_tcase(s, tc);
  return s;
}