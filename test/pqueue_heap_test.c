#include <check.h>

#include "../src/pqueue.h"
#include "libccg_test.h"

int cmpint(const int *a, const int *b) {
  if (*a < *b)
    return -1;
  else if (*a > *b)
    return 1;
  else
    return 0;
}

START_TEST(pqueue_create) {
  pqueue *queue;

  queue = ccg_pqueue_create((int (*)(const void *, const void *))cmpint);
  ck_assert_ptr_nonnull(queue);
  ck_assert_int_eq(ccg_pqueue_size(queue), 0);
  ccg_pqueue_destroy(queue);
}
END_TEST

START_TEST(pqueue_insert_int) {
  int *a, *b, *c, *d, *e, *f;
  pqueue *queue;

  //*a = 1, *b = 2, *c = 3, *d = 4, *e = 5, *f = 6;
  //queue = ccg_pqueue_create((int (*)(const void *, const void *))cmpint);
  //ccg_pqueue_insert(a, queue);
  //ck_assert_int_eq(ccg_pqueue_size(queue), 1);
  //ccg_pqueue_insert(b, queue);
  //ck_assert_int_eq(ccg_pqueue_size(queue), 2);
  //ccg_pqueue_insert(c, queue);
  //ck_assert_int_eq(ccg_pqueue_size(queue), 3);
  //ccg_pqueue_insert(d, queue);
  //ck_assert_int_eq(ccg_pqueue_size(queue), 4);
  //ccg_pqueue_insert(e, queue);
  //ck_assert_int_eq(ccg_pqueue_size(queue), 5);
  //ccg_pqueue_insert(f, queue);
  //ck_assert_int_eq(ccg_pqueue_size(queue), 6);
  //ccg_pqueue_destroy(queue);
}
END_TEST

Suite *pqueue_heap_suite() {
  Suite *s;
  TCase *tc;

  s = suite_create("Priority Queue (heap)");
  tc = tcase_create("Core");

  tcase_add_test(tc, pqueue_create);
  tcase_add_test(tc, pqueue_insert_int);

  suite_add_tcase(s, tc);
  return s;
}