#include <CUnit/Basic.h>
#include "Generic_func_Data_types/common.h"
#include "Generic_func_Data_types/store_specific_data_types.h"
#include "hash_table.h"
#include "linked_list.h"
#include "iterator.h"
#include "shopping_cart.h"



static int init_suite(void)
{
  // Change this function if you want to do something *before* you
  // run a test suite
  return 0;
}

static int clean_suite(void)
{
  // Change this function if you want to do something *after* you
  // run a test suite
  return 0;
}

// These are example test functions. You should replace them with
// functions of your own.



static void test_is_menu_char_false(void){
    CU_ASSERT_TRUE(is_menu_char("L"))
}

static void test_is_menu_char_false(void){
    CU_ASSERT_FALSE(is_menu_char("p"))
}

int main()
{
  // First we try to set up CUnit, and exit if we fail
  if (CU_initialize_registry() != CUE_SUCCESS)
    return CU_get_error();

  // We then create an empty test suite and specify the name and
  // the init and cleanup functions
  CU_pSuite my_test_suite = CU_add_suite("My awesome test suite", init_suite, clean_suite);
  if (my_test_suite == NULL)
  {
    // If the test suite could not be added, tear down CUnit and exit
    CU_cleanup_registry();
    return CU_get_error();
  }

  // This is where we add the test functions to our test suite.
  // For each call to CU_add_test we specify the test suite, the
  // name or description of the test, and the function that runs
  // the test in question. If you want to add another test, just
  // copy a line below and change the information
  if (
      (CU_add_test(my_test_suite, "False test of ioopm_is_shelf", test_is_shelf) == NULL) || 0)
  {
    // If adding any of the tests fails, we tear down CUnit and exit
    CU_cleanup_registry();
    return CU_get_error();
  }

  //(CU_add_test(my_test_suite, "ht has_value", test_hash_table_has_value) == NULL)||
  // Set the running mode. Use CU_BRM_VERBOSE for maximum output.
  // Use CU_BRM_NORMAL to only print errors and a summary
  CU_basic_set_mode(CU_BRM_NORMAL);

  // This is where the tests are actually run!

  CU_basic_run_tests();

  // Tear down CUnit before exiting
  CU_cleanup_registry();
  return CU_get_error();
}