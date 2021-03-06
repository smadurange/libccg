#include <assert.h>
#include <check.h>

#include "../../src/adt/pqueue.h"
#include "../../src/mem.h"
#include "../libccg_test.h"

static int cmpint(const int *a, const int *b) {
	if (a == b || *a == *b)
		return 0;
	else if (*a < *b)
		return -1;
	else
		return 1;
}

START_TEST(test_pqueue_create) {
	pqueue *queue;

	queue = ccg_pqueue_create((cmp)cmpint, 0);
	ck_assert_ptr_nonnull(queue);
	ck_assert_int_eq(ccg_pqueue_size(queue), 0);

	ccg_pqueue_destroy(queue);
}
END_TEST

START_TEST(test_pqueue_insert_int) {
	int a, b, c, d, e, f;
	pqueue *queue;

	a = 1, b = 2, c = 3, d = 4, e = 4, f = 5;
	queue = ccg_pqueue_create((cmp)cmpint, 0);

	ccg_pqueue_insert(&a, queue);
	ck_assert_int_eq(ccg_pqueue_size(queue), 1);
	ccg_pqueue_insert(&b, queue);
	ck_assert_int_eq(ccg_pqueue_size(queue), 2);
	ccg_pqueue_insert(&c, queue);
	ck_assert_int_eq(ccg_pqueue_size(queue), 3);
	ccg_pqueue_insert(&d, queue);
	ck_assert_int_eq(ccg_pqueue_size(queue), 4);
	ccg_pqueue_insert(&e, queue);
	ck_assert_int_eq(ccg_pqueue_size(queue), 5);
	ccg_pqueue_insert(&f, queue);
	ck_assert_int_eq(ccg_pqueue_size(queue), 6);

	ccg_pqueue_destroy(queue);
}
END_TEST

START_TEST(test_pqueue_remove_int) {
	pqueue *queue;
	int a, b, c, d, e, f, *rv;

	a = 1, b = 8, c = 3, d = -1, e = 5, f = 3;
	queue = ccg_pqueue_create((cmp)cmpint, 0);
	ccg_pqueue_insert(&a, queue);
	ccg_pqueue_insert(&b, queue);
	ccg_pqueue_insert(&c, queue);
	ccg_pqueue_insert(&d, queue);
	ccg_pqueue_insert(&e, queue);
	ccg_pqueue_insert(&f, queue);

	rv = ccg_pqueue_remove(queue);
	ck_assert_int_eq(*rv, b);
	ck_assert_int_eq(ccg_pqueue_size(queue), 5);
	rv = ccg_pqueue_remove(queue);
	ck_assert_int_eq(*rv, e);
	ck_assert_int_eq(ccg_pqueue_size(queue), 4);
	rv = ccg_pqueue_remove(queue);
	ck_assert_int_eq(*rv, c);
	ck_assert_int_eq(ccg_pqueue_size(queue), 3);
	rv = ccg_pqueue_remove(queue);
	ck_assert_int_eq(*rv, f);
	ck_assert_int_eq(ccg_pqueue_size(queue), 2);
	rv = ccg_pqueue_remove(queue);
	ck_assert_int_eq(*rv, a);
	ck_assert_int_eq(ccg_pqueue_size(queue), 1);
	rv = ccg_pqueue_remove(queue);
	ck_assert_int_eq(*rv, d);
	ck_assert_int_eq(ccg_pqueue_size(queue), 0);

	ccg_pqueue_destroy(queue);
}
END_TEST

static void clsint(void *ptr) { ccg_free(ptr); }

START_TEST(test_pqueue_destroy) {
	int *a;
	pqueue *pq;

	a = ccg_malloc(sizeof(int));
	*a = 1;
	pq = ccg_pqueue_create((cmp)cmpint, clsint);
	ccg_pqueue_insert(a, pq);
	ccg_pqueue_destroy(pq);
}
END_TEST

Suite *pqueue_suite() {
	Suite *s;
	TCase *tc;

	s = suite_create("priority queue");
	tc = tcase_create("core");

	tcase_add_test(tc, test_pqueue_create);
	tcase_add_test(tc, test_pqueue_insert_int);
	tcase_add_test(tc, test_pqueue_remove_int);
	tcase_add_test(tc, test_pqueue_destroy);

	suite_add_tcase(s, tc);
	return s;
}
