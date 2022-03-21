#include <check.h>

#include "libccg_test.h"
#include "../src/pqueue.h"

int cmp(int *a, int *b) {
  return *a > *b;
}

START_TEST(pqueue_create) {
  pqueue *queue;

  queue = ccg_pqueue_create((int (*)(void *, void*))cmp);
  ck_assert_ptr_nonnull(queue);
  ck_assert_int_eq(ccg_pqueue_size(queue), 0);
}
END_TEST

Suite *pqueue_heap_suite() {
  Suite *s;
  TCase *tc;

  s = suite_create("Priority Queue (heap)");
  tc = tcase_create("Core");

  tcase_add_test(tc, pqueue_create);

  suite_add_tcase(s, tc);
  return s;
}