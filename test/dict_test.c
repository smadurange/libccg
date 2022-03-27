#include <check.h>

#include "../src/dict.h"
#include "../src/mem.h"
#include "libccg_test.h"

static int hashint(const int *i, const size_t n) { return *i % n; }

static int cmpint(const int *a, const int *b) {
	if (a == b || *a == *b)
		return 1;
	else
		return 0;
}

static void finint(int *i) { ccg_free(i); }

START_TEST(test_dict_create) {
	dict *dt;

	dt = ccg_dict_create((hasher)hashint, (comparer)cmpint, 0);
	ck_assert_ptr_nonnull(dt);

	ccg_dict_destroy(dt);
}
END_TEST

START_TEST(test_dict_put) {
	dict *dt;
	int *k, *v;

	k = ccg_malloc(sizeof(int));
	v = ccg_malloc(sizeof(int));
	*k = 1, *v = 2;
	dt = ccg_dict_create((hasher)hashint, (comparer)cmpint, (finalizer)finint);

	ccg_dict_put(k, v, dt);
	ck_assert_int_eq(*(int *)ccg_dict_find(k, dt), *v);

	ccg_dict_destroy(dt);
}

Suite *dict_suite() {
	Suite *s;
	TCase *tc;

	s = suite_create("dictionary");
	tc = tcase_create("core");

	tcase_add_test(tc, test_dict_create);
	tcase_add_test(tc, test_dict_put);

	suite_add_tcase(s, tc);
	return s;
}
