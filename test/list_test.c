#include <check.h>

#include "../src/list.h"
#include "../src/mem.h"
#include "libccg_test.h"

static int cmpint(const int *a, const int *b) {
  if (*a < *b)
    return -1;
  else if (*a > *b)
    return 1;
  else
    return 0;
}

START_TEST(test_list_create) {
  list *ls;

  ls = ccg_list_create();
  ck_assert_ptr_nonnull(ls);

  ccg_list_destroy(ls);
}
END_TEST

Suite *list_suite() {
  Suite *s;
  TCase *tc;

  s = suite_create("linked list");
  tc = tcase_create("core");

  tcase_add_test(tc, test_list_create);

  suite_add_tcase(s, tc);
  return s;
}