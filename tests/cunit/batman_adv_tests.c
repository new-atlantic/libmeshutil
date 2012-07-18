#ifdef __linux

#include <stdlib.h>

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#define UNIMPLMENTED "Test not implemented"

void check_kernel_module_available (void) {
  CU_FAIL (UNIMPLMENTED);
}

int main (void)
{
	unsigned int failures;
	CU_pSuite pSuite = NULL;

	if (CU_initialize_registry() != CUE_SUCCESS)
          return CU_get_error();

	pSuite = CU_add_suite ("meshutil batman_adv suite", NULL, NULL);

	if (!pSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	if (!CU_add_test (pSuite,
	                  "Test whether checking for the availability of mod"
                          " batman_adv works",
	                  check_kernel_module_available)) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	CU_basic_set_mode (CU_BRM_VERBOSE);
	CU_basic_run_tests();
	failures = CU_get_number_of_failures();
	CU_cleanup_registry();

	if (failures > 0) {
		return EXIT_FAILURE;
	} else {
		return EXIT_SUCCESS;
	}
}

#endif /* __linux */
