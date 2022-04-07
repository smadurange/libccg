#include <check.h>

#include "../src/point.h"
#include "libccg_test.h"

START_TEST(test_point_eq) {
	point a, b, c;

	a.x = 0.1;
	a.y = 0.2;
	b.x = 0.1;
	b.y = 0.2;
	c.x = 0.0;
	c.y = 1.0;

	ck_assert_int_eq(ccg_point_eq(&a, &a, 0.1), 1);
	ck_assert_int_eq(ccg_point_eq(&a, &b, 0.1), 1);
	ck_assert_int_eq(ccg_point_eq(&a, &c, 0.1), 0);
	ck_assert_int_eq(ccg_point_eq(&a, &c, 100), 1);
}
END_TEST

Suite *point_suite() {
	Suite *s;
	TCase *tc;

	s = suite_create("point2d");
	tc = tcase_create("core");

	tcase_add_test(tc, test_point_eq);

	suite_add_tcase(s, tc);
	return s;
}
