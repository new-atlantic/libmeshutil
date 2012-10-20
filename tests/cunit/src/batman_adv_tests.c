#ifdef __linux

#define _XOPEN_SOURCE 700

#include <stdlib.h>
#include <sys/utsname.h>

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "batman_adv.c"

#define UNIMPLMENTED "Test not implemented"

void check_module_version (void) {

     char *version = mu_badv_kmod_version(NULL);

     if (mu_badv_kmod_loaded(NULL)) {
          CU_ASSERT_PTR_NOT_NULL(version);
          free(version);
     } else {
          CU_ASSERT_PTR_NULL(version)
               }

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
                       "Test whether checking for batman_adv version works",
                       check_module_version)) {
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
