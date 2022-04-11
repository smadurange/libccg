#include <check.h>

#include "../src/voronoi.h"
#include "libccg_test.h"

START_TEST(test_voronoi_solve_basic) {
	point bbox[4], sites[5];
	voronoi_diagram *vd;

	bbox[0] = (point){0.0, 2.0};
	bbox[1] = (point){18.0, 2.0};
	bbox[2] = (point){18.0, 14.0};
	bbox[3] = (point){0.0, 14.0};

	sites[0] = (point){6.48, 4.7};
	sites[1] = (point){13.9, 6.76};
	sites[2] = (point){7.12, 8.1};
	sites[3] = (point){12.7, 10.6};
	sites[4] = (point){4.6, 11.44};

	vd = ccg_voronoi_solve(sites, 5, bbox);

	ck_assert_ptr_nonnull(vd);

	ccg_voronoi_destroy(vd);
}
END_TEST

Suite *voronoi_suite() {
	Suite *s;
	TCase *tc;

	s = suite_create("voronoi");
	tc = tcase_create("core");

	tcase_add_test(tc, test_voronoi_solve_basic);

	suite_add_tcase(s, tc);
	return s;
}
