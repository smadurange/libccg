#include <check.h>
#include <string.h>

#include "../../src/adt/dict.h"
#include "../../src/mem.h"
#include "../libccg_test.h"

static int hashint(const int *i, const size_t n) { return *i % n; }

static int hashstr(const char *s, const size_t n) {
	int h, a, b;

	a = 31415, b = 27189;
	for (h = 0; *s; s++, a = a * b % (n - 1))
		h = (a * h + *s) % n;
	return h;
}

static int cmpint(const int *a, const int *b) { return !(a == b || *a == *b); }

static void clsint(int *i) { ccg_free(i); }

START_TEST(test_dict_create) {
	dict *dt;

	dt = ccg_dict_create((hash)hashint, (cmp)cmpint, 0);
	ck_assert_ptr_nonnull(dt);

	ccg_dict_destroy(dt);
}
END_TEST

START_TEST(test_dict_put) {
	dict *dt;
	int k, v;

	k = 1, v = 2;
	dt = ccg_dict_create((hash)hashint, (cmp)cmpint, (cls)clsint);

	ccg_dict_put(&k, &v, dt);
	ck_assert_int_eq(*(int *)ccg_dict_find(&k, dt), v);

	ccg_dict_destroy(dt);
}
END_TEST

START_TEST(test_dict_find) {
	dict *dt;
	char *k1, v1, *k2, v2, *k3, v3;

	k1 = "a", v1 = 1;
	k2 = "b", v2 = 2;
	k3 = "b", v2 = 3;
	dt = ccg_dict_create((hash)hashstr, (cmp)strcmp, 0);

	ccg_dict_put(k1, &v1, dt);
	ccg_dict_put(k2, &v2, dt);
	ccg_dict_put(k3, &v3, dt);

	ck_assert_ptr_eq(ccg_dict_find("c", dt), 0);
	ck_assert_int_eq(*(char *)ccg_dict_find(k1, dt), v1);
	ck_assert_int_eq(*(char *)ccg_dict_find(k2, dt), v2);
	ck_assert_int_eq(*(char *)ccg_dict_find(k3, dt), v2);

	ccg_dict_destroy(dt);
}
END_TEST

START_TEST(test_dict_remove) {
	dict *dt;
	char *k, v;

	k = "a", v = 1;
	dt = ccg_dict_create((hash)hashstr, (cmp)strcmp, 0);
	ccg_dict_put(k, &v, dt);

	ck_assert_ptr_eq(ccg_dict_remove("b", dt), 0);
	ck_assert_int_eq(*(char *)ccg_dict_remove(k, dt), v);
	ck_assert_ptr_eq(ccg_dict_remove(k, dt), 0);

	ccg_dict_destroy(dt);
}
END_TEST

START_TEST(test_dict_destroy) {
	dict *dt;
	char *k, *v;

	v = ccg_malloc(sizeof(char));
	k = "a", *v = 1;
	dt = ccg_dict_create((hash)hashstr, (cmp)strcmp, (cls)clsint);
	ccg_dict_put(k, &v, dt);

	ccg_dict_destroy(dt);
}
END_TEST

Suite *dict_suite() {
	Suite *s;
	TCase *tc;

	s = suite_create("dictionary");
	tc = tcase_create("core");

	tcase_add_test(tc, test_dict_create);
	tcase_add_test(tc, test_dict_put);
	tcase_add_test(tc, test_dict_find);
	tcase_add_test(tc, test_dict_remove);
	tcase_add_test(tc, test_dict_destroy);

	suite_add_tcase(s, tc);
	return s;
}
