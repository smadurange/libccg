#include <check.h>

#include "../src/list.h"
#include "../src/mem.h"
#include "libccg_test.h"

static int cmpint(const int *a, const int *b) {
  if (a == b || *a == *b)
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

START_TEST(test_list_find_or_append) {
  void *rv;
  list *ls;
  int *a, *b;

  a = ccg_malloc(sizeof(int));
  b = ccg_malloc(sizeof(int));
  *a = 0, *b = 1;
  ls = ccg_list_create();

  rv = ccg_list_find_or_append(a, (cmp)cmpint, ls);
  ck_assert_ptr_null(rv);
  rv = ccg_list_find_or_append(b, (cmp)cmpint, ls);
  ck_assert_ptr_null(rv);
  rv = ccg_list_find_or_append(b, (cmp)cmpint, ls);
  ck_assert_int_eq(*(int *)rv, *b);

  ccg_free(a);
  ccg_free(b);
  ccg_list_destroy(ls);
}
END_TEST

Suite *list_suite() {
  Suite *s;
  TCase *tc;

  s = suite_create("linked list");
  tc = tcase_create("core");

  tcase_add_test(tc, test_list_create);
  tcase_add_test(tc, test_list_find_or_append);

  suite_add_tcase(s, tc);
  return s;
}