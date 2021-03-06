#include <check.h>

#include "../../src/adt/list.h"
#include "../../src/mem.h"
#include "../libccg_test.h"

static int cmpint(const int *a, const int *b) { return !(a == b || *a == *b); }

START_TEST(test_list_create) {
	list *ls;

	ls = ccg_list_create();
	ck_assert_ptr_nonnull(ls);

	ccg_list_destroy(0, ls);
}
END_TEST

START_TEST(test_list_find_or_append) {
	list *ls;
	int a, b, *rv;

	a = 0, b = 1;
	ls = ccg_list_create();

	rv = ccg_list_put_if_absent(&a, (cmp)cmpint, ls);
	ck_assert_ptr_null(rv);
	rv = ccg_list_put_if_absent(&b, (cmp)cmpint, ls);
	ck_assert_ptr_null(rv);
	rv = ccg_list_put_if_absent(&b, (cmp)cmpint, ls);
	ck_assert_int_eq(*rv, b);

	ccg_list_destroy(0, ls);
}
END_TEST

START_TEST(test_list_find) {
	list *ls;
	int a, b, c, d;

	a = 0, b = 100, c = 3, d = 7;
	ls = ccg_list_create();
	ccg_list_put_if_absent(&a, (cmp)cmpint, ls);
	ccg_list_put_if_absent(&b, (cmp)cmpint, ls);
	ccg_list_put_if_absent(&c, (cmp)cmpint, ls);

	ck_assert_ptr_null(ccg_list_find(&d, (cmp)cmpint, ls));
	ck_assert_int_eq(*(int *)ccg_list_find(&b, (cmp)cmpint, ls), b);

	ccg_list_destroy(0, ls);
}
END_TEST

START_TEST(test_list_remove) {
	list *ls;
	int a, b, c;

	a = 13, b = 5, c = 100;
	ls = ccg_list_create();

	ck_assert_ptr_null(ccg_list_remove(&a, (cmp)cmpint, &ls));

	ccg_list_put_if_absent(&a, (cmp)cmpint, ls);
	ccg_list_put_if_absent(&b, (cmp)cmpint, ls);
	ccg_list_put_if_absent(&c, (cmp)cmpint, ls);

	ck_assert_int_eq(*(int *)ccg_list_remove(&a, (cmp)cmpint, &ls), a);
	ck_assert_int_eq(*(int *)ccg_list_remove(&c, (cmp)cmpint, &ls), c);
	ck_assert_ptr_null(ccg_list_remove(&a, (cmp)cmpint, &ls));

	ccg_list_destroy(0, ls);
}
END_TEST

static void clsint(void *ptr) { ccg_free(ptr); }

START_TEST(test_list_destroy) {
	int *a;
	list *ls;

	a = ccg_malloc(sizeof(int));
	*a = 1;
	ls = ccg_list_create();
	ccg_list_put_if_absent(&a, (cmp)cmpint, ls);
	ccg_list_destroy((cls)clsint, ls);
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
	tcase_add_test(tc, test_list_destroy);

	suite_add_tcase(s, tc);
	return s;
}
