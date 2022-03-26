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

  rv = ccg_list_find_or_append(a, (comparer)cmpint, ls);
  ck_assert_ptr_null(rv);
  rv = ccg_list_find_or_append(b, (comparer)cmpint, ls);
  ck_assert_ptr_null(rv);
  rv = ccg_list_find_or_append(b, (comparer)cmpint, ls);
  ck_assert_int_eq(*(int *)rv, *b);

  ccg_free(a);
  ccg_free(b);
  ccg_list_destroy(ls);
}
END_TEST

START_TEST(test_list_find) {
  list *ls;
  int *a, *b, *c, *d;

  a = ccg_malloc(sizeof(int));
  b = ccg_malloc(sizeof(int));
  c = ccg_malloc(sizeof(int));
  d = ccg_malloc(sizeof(int));
  *a = 0, *b = 100, *c = 3, *d = 7;
  ls = ccg_list_create();

  ccg_list_find_or_append(a, (comparer)cmpint, ls);
  ccg_list_find_or_append(b, (comparer)cmpint, ls);
  ccg_list_find_or_append(c, (comparer)cmpint, ls);
  ck_assert_ptr_null(ccg_list_find(d, (comparer)cmpint, ls));
  ck_assert_int_eq(*(int *)ccg_list_find(b, (comparer)cmpint, ls), *b);

  ccg_free(a);
  ccg_free(b);
  ccg_free(c);
  ccg_free(d);
  ccg_list_destroy(ls);
}
END_TEST

START_TEST(test_list_remove) {
  list *ls;
  int *a, *b, *c;

  a = ccg_malloc(sizeof(int));
  b = ccg_malloc(sizeof(int));
  c = ccg_malloc(sizeof(int));
  *a = 13, *b = 5, *c = 100;
  ls = ccg_list_create();

  ck_assert_ptr_null(ccg_list_remove(a, (comparer)cmpint, &ls));

  ccg_list_find_or_append(a, (comparer)cmpint, ls);
  ccg_list_find_or_append(b, (comparer)cmpint, ls);
  ccg_list_find_or_append(c, (comparer)cmpint, ls);

  ck_assert_int_eq(*(int *)ccg_list_remove(a, (comparer)cmpint, &ls), *a);
  ck_assert_int_eq(*(int *)ccg_list_remove(c, (comparer)cmpint, &ls), *c);
  ck_assert_ptr_null(ccg_list_remove(a, (comparer)cmpint, &ls));

  ccg_free(a);
  ccg_free(b);
  ccg_free(c);
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
  tcase_add_test(tc, test_list_find);
  tcase_add_test(tc, test_list_remove);

  suite_add_tcase(s, tc);
  return s;
}