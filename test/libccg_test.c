#include <check.h>
#include <stdlib.h>

#include "libccg_test.h"

int main() {
	int fc;
	SRunner *sr;

	sr = srunner_create(pqueue_suite());
	srunner_add_suite(sr, list_suite());
	srunner_add_suite(sr, dict_suite());

	srunner_run_all(sr, CK_NORMAL);
	fc = srunner_ntests_failed(sr);
	srunner_free(sr);

	return fc == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
