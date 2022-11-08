// libraries
#include <CUnit/Basic.h>
#include "items_db.h"
// #include "store.h"

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

// // These are example test functions. You should replace them with
// // functions of your own.
static void test_is_shelf(){
  char *shelf = "A12";
  char *shelf_false = "AAA";


  CU_ASSERT_TRUE(ioopm_is_shelf(shelf));
  CU_ASSERT_FALSE(ioopm_is_shelf(shelf_false));

}

static void test_item_unique(){
  ioopm_warehouse_t warehouse = ioopm_create_warehouse();
  ioopm_warehouse_t warehouse_empty = ioopm_create_warehouse();
  ioopm_item_t *item = ioopm_make_item_backend("ape", "a ape", 2);
  ioopm_item_t *item_new = ioopm_make_item_backend("ape", "a ape", 2);

  bool check = ioopm_add_item(warehouse.HTn, item);

  CU_ASSERT_TRUE(ioopm_item_name_unique(warehouse.HTn, item_new->name));
  CU_ASSERT_TRUE(ioopm_item_name_unique(warehouse_empty.HTn, item_new->name));
  CU_ASSERT_FALSE(ioopm_item_name_unique(warehouse.HTn, item->name));
  

}

static void test_random(void){
  char *random = random_shelf();

  CU_ASSERT_TRUE(ioopm_is_shelf(random));
}


static void test_remove_item(){
  ioopm_warehouse_t warehouse = ioopm_create_warehouse();
  ioopm_item_t *item = ioopm_make_item_backend("ape", "a ape", 2);
  ioopm_item_t *item_new = ioopm_make_item_backend("ape", "a ape", 2);

  CU_ASSERT_EQUAL(ioopm_hash_table_size(warehouse.HTn), 0);

  ioopm_add_item(warehouse.HTn, item);

  CU_ASSERT_FALSE(ioopm_remove_item(&warehouse, item_new));
  CU_ASSERT_EQUAL(ioopm_hash_table_size(warehouse.HTn), 1);
  

  CU_ASSERT_TRUE(ioopm_remove_item(&warehouse,item));
  CU_ASSERT_EQUAL(ioopm_hash_table_size(warehouse.HTn), 0);  
}

static void test_replenish_stock(){
  ioopm_warehouse_t warehouse = ioopm_create_warehouse();
  ioopm_item_t *item = ioopm_make_item_backend("ape", "a ape", 2);
  ioopm_item_t *item_new = ioopm_make_item_backend("ape", "a ape", 2);

  CU_ASSERT_EQUAL(0, ioopm_hash_table_size(warehouse.HTsl));
  CU_ASSERT_FALSE(ioopm_replenish_stock(&warehouse, item, 0));
  CU_ASSERT_EQUAL(0, ioopm_hash_table_size(warehouse.HTsl));

  CU_ASSERT_TRUE(ioopm_replenish_stock(&warehouse, item, 3));
  CU_ASSERT_EQUAL(3, ioopm_hash_table_size(warehouse.HTsl));
  CU_ASSERT_TRUE(ioopm_replenish_stock(&warehouse, item_new, 4));
  CU_ASSERT_EQUAL(7, ioopm_hash_table_size(warehouse.HTsl));

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

      0)
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