#include <CUnit/Basic.h>
#include "shopping_cart.h"



static void test_create(){
    ioopm_hash_table_t *cart = create_cart(); // TODO: create_cart returnar inte ioopm_hash_table_t
    CU_ASSERT_NOT_NULL(cart);

    ioopm_hash_table_t *cart_list = ioopm_create_cart_list();
    CU_ASSERT_NOT_NULL(cart_list);
}

static void test_destroy(){
    ioopm_hash_table_t *cart = create_cart();
    ioopm_hash_table_t *all_carts = ioopm_create_cart_list();
    
    CU_ASSERT_NOT_NULL(all_carts);

    ioopm_destroy_cart_list(all_carts);

    CU_ASSERT_NULL(all_carts);
}

void test_add_to_cart()
{
    
    ioopm_hash_table_t *cart = create_cart(); 
    
    ioopm_item_t item = make_item_backend("ape","a ape", 2);
    ioopm_add_to_cart(cart,item, 2);
    CU_ASSERT_TRUE(ioopm_is_in_shopping_cart(cart, item));
    int amount = ioopm_amount_items_in_cart(cart, item);
    CU_ASSERT_EQUAL(amount, 2)
    
}

static void test_remove_cart_cart_items(){
    ioopm_hash_table_t *cart = create_cart();
    ioopm_item_t item = make_item_backend("ape","a ape", 2);
    ioopm_add_to_cart(cart,item, 2);

    CU_ASSERT_PTR_NOT_NULL(cart);
    CU_ASSERT_TRUE(ioopm_is_in_shopping_cart(cart, item));
    CU_ASSERT_EQUAL(ioopm_amount_items_in_cart(cart, item), 2);

    ioopm_remove_from_cart(cart, item);

    CU_ASSERT_FALSE(ioopm_is_in_shopping_cart(cart, item));
    CU_ASSERT_EQUAL(ioopm_amount_items_in_cart(cart, item), 0);

    CU_ASSERT_PTR_NOT_NULL(cart);
    ioopm_destroy_cart(cart);
    CU_ASSERT_NULL(cart);
}

static void test_calculate_cost(){
    ioopm_hash_table_t *cart = create_cart();
    ioopm_item_t item = make_item_backend("ape","a ape", 2);
    ioopm_add_to_cart(cart,item, 2);

    int cost = calculate_cost(cart);
    CU_ASSERT_EQUAL(cost, 4);
}

static void test_calculate_cost_empty(){
    ioopm_hash_table_t *cart = create_cart(); 

    int cost = calculate_cost(cart);
    CU_ASSERT_EQUAL(cost, 0);
}

static void test_checkout(){
    ioopm_warehouse_t *warehouse = ioopm_create_warehouse();
    ioopm_hash_table_t *cart = create_cart();
    ioopm_item_t item = make_item_backend("ape","a ape", 2);

    ioopm_add_item(warehouse->HTn, item);
    replenish_stock(warehouse, item);
    ioopm_add_to_cart(cart, item 1);
    checkout(cart, warehouse->HTsl);

    
    CU_ASSERT_EQUAL(stock_location , ioopm_hash_table_size(warehouse->HTsl));

}

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
static void test1(void)
{
    CU_ASSERT(42);
}

static void test2(void)
{
    CU_ASSERT_EQUAL(1 + 1, 2);
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
        (CU_add_test(my_test_suite, "A simple test", test1) == NULL) ||
        (CU_add_test(my_test_suite, "Basic arithmetics", test2) == NULL) ||
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
