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

START_TEST(test_dict_create) {
	dict *dt;

	dt = ccg_dict_create((hasher)hashint, (comparer)cmpint, 0);
	ck_assert_ptr_nonnull(dt);

	ccg_dict_destroy(dt);
}
END_TEST

Suite *dict_suite() {
	Suite *s;
	TCase *tc;

	s = suite_create("dictionary");
	tc = tcase_create("core");

	tcase_add_test(tc, test_dict_create);

	suite_add_tcase(s, tc);
	return s;
}
