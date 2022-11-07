#include <CUnit/Basic.h>
#include "shopping_cart.h"
#include "db_back_end.h"

#include "../db.h"




static void test_create()
{
    ioopm_shopping_cart_t *cart = create_cart(100);
    CU_ASSERT_PTR_NOT_NULL(cart);

    ioopm_hash_table_t *all_carts = ioopm_create_cart_list();
    CU_ASSERT_PTR_NOT_NULL(all_carts);

    ioopm_destroy_cart(cart);
    ioopm_destroy_cart_list(all_carts);
}

static void test_destroy()
{
    ioopm_shopping_cart_t *cart = create_cart(100);
    ioopm_hash_table_t *all_carts = ioopm_create_cart_list();

    CU_ASSERT_PTR_NOT_NULL(all_carts);
    CU_ASSERT_PTR_NOT_NULL(cart);

    CU_ASSERT_TRUE(ioopm_destroy_cart(cart));
    CU_ASSERT_TRUE(ioopm_destroy_cart_list(all_carts));

    CU_ASSERT_FALSE(ioopm_destroy_cart(NULL));
    CU_ASSERT_FALSE(ioopm_destroy_cart_list(NULL));
}

static void test_id_and_eq_cart()
{
    ioopm_hash_table_t *all_carts = ioopm_create_cart_list();
    int id = 123;
    int random_id = 0;
    create_cart_in_cart_list(all_carts, id);

    ioopm_shopping_cart_t *cart = choose_cart(all_carts, id);
    CU_ASSERT_EQUAL(ioopm_cart_id(cart), id);
    CU_ASSERT_NOT_EQUAL(ioopm_cart_id(cart), random_id);

    
    ioopm_destroy_cart_list(all_carts);
}

static void test_cart_unique()
{
    ioopm_hash_table_t *all_carts = ioopm_create_cart_list();
    ioopm_hash_table_t *all_carts_empty = ioopm_create_cart_list();
    int existing_id = 1231;
    int random_id = 0; // unset
    create_cart_in_cart_list(all_carts, existing_id);

    CU_ASSERT_FALSE(cart_unique(all_carts, existing_id));
    CU_ASSERT_TRUE(cart_unique(all_carts, random_id));

    CU_ASSERT_TRUE(cart_unique(all_carts_empty, random_id));

    ioopm_destroy_cart_list(all_carts);
    ioopm_destroy_cart_list(all_carts_empty);
}

static void test_amount_items_in_cart()
{
    ioopm_warehouse_t warehouse = ioopm_create_warehouse();
    ioopm_shopping_cart_t *cart = create_cart(10);
    ioopm_shopping_cart_t *cart_empty = create_cart(10);
    ioopm_item_t *item = make_item_backend(strdup("ape"), strdup("a ape"), 2);
    ioopm_item_t *item_not_in_cart = make_item_backend(strdup("dante"), strdup("a dante"), 2);
    replenish_stock(&warehouse, item, 2);
    ioopm_add_to_cart(cart, item, 2);

    CU_ASSERT_EQUAL(2, ioopm_amount_items_in_cart(cart, item));
    CU_ASSERT_EQUAL(0, ioopm_amount_items_in_cart(cart, item_not_in_cart));
    CU_ASSERT_EQUAL(0, ioopm_amount_items_in_cart(cart_empty, item));

    ioopm_destroy_cart(cart);
    ioopm_destroy_cart(cart_empty);
    ioopm_destroy_item(item);
    ioopm_destroy_item(item_not_in_cart);
}

static void test_is_in_shopping_cart()
{   
    ioopm_warehouse_t warehouse = ioopm_create_warehouse();
    ioopm_shopping_cart_t *cart = create_cart(100);
    ioopm_shopping_cart_t *cart_empty = create_cart(100);

    ioopm_item_t *item = make_item_backend(strdup("ape"), strdup("a ape"), 2);
    ioopm_item_t *item_new = make_item_backend(strdup("dante"), strdup("a dante"), 2);
    replenish_stock(&warehouse, item, 2);
    replenish_stock(&warehouse, item, 2);
    ioopm_add_to_cart(cart, item, 2);

    CU_ASSERT_TRUE(ioopm_is_in_shopping_cart(cart, item));
    CU_ASSERT_FALSE(ioopm_is_in_shopping_cart(cart, item_new));
    CU_ASSERT_FALSE(ioopm_is_in_shopping_cart(cart_empty, item));
    CU_ASSERT_FALSE(ioopm_is_in_shopping_cart(cart_empty, item_new));

    ioopm_destroy_cart(cart);
    ioopm_destroy_cart(cart_empty);
    ioopm_destroy_item(item);
    ioopm_destroy_item(item_new);
}

static void test_choose_cart()
{
    ioopm_hash_table_t *all_carts = ioopm_create_cart_list();
    ioopm_hash_table_t *all_carts_empty = ioopm_create_cart_list();
    int existing_id = 1231;
    int random_id_one = 123123;
    int random_id_two = 12331223;

    create_cart_in_cart_list(all_carts, existing_id);
    create_cart_in_cart_list(all_carts, random_id_one);
    create_cart_in_cart_list(all_carts, random_id_two);

    ioopm_shopping_cart_t *found_cart = ioopm_hash_table_lookup(all_carts, ioopm_int_to_elem(existing_id)).value.p;
    ioopm_shopping_cart_t *choosen_cart = choose_cart(all_carts, existing_id);

    CU_ASSERT_EQUAL(found_cart, choosen_cart);
    ioopm_destroy_cart_list(all_carts);
    ioopm_destroy_cart_list(all_carts_empty);
}

static void test_add_to_cart()
{
    ioopm_warehouse_t warehouse = ioopm_create_warehouse();
    ioopm_shopping_cart_t *cart = create_cart(100);
    ioopm_shopping_cart_t *cart_new = create_cart(100);
    ioopm_shopping_cart_t *cart_empty = create_cart(100);
    ioopm_item_t *item = make_item_backend(strdup("ape"), strdup("a ape"), 2);
    ioopm_item_t *item_new = make_item_backend(strdup("dante"), strdup("a ape"), 2);

    replenish_stock(&warehouse, item, 2);
    ioopm_add_to_cart(cart, item, 2);
    ioopm_add_to_cart(cart_new, item_new, 0);

    CU_ASSERT_TRUE(ioopm_is_in_shopping_cart(cart, item));
    CU_ASSERT_EQUAL(2, ioopm_amount_items_in_cart(cart, item));

    CU_ASSERT_FALSE(ioopm_is_in_shopping_cart(cart_new, item_new));
    CU_ASSERT_EQUAL(0, ioopm_amount_items_in_cart(cart_new, item_new));

    CU_ASSERT_FALSE(ioopm_is_in_shopping_cart(cart_empty, item));
    CU_ASSERT_FALSE(ioopm_is_in_shopping_cart(cart_empty, item_new));
    CU_ASSERT_EQUAL(0, ioopm_amount_items_in_cart(cart_empty, item));
    CU_ASSERT_EQUAL(0, ioopm_amount_items_in_cart(cart_empty, item_new));

    ioopm_destroy_cart(cart);
    ioopm_destroy_cart(cart_new);
    ioopm_destroy_cart(cart_empty);
    ioopm_destroy_item(item);
    ioopm_destroy_item(item_new);
}

static void test_remove_cart_items()
{   
    ioopm_warehouse_t warehouse = ioopm_create_warehouse();
    ioopm_shopping_cart_t *cart = create_cart(100);
    ioopm_item_t *item = make_item_backend(strdup("ape"), strdup("a ape"), 2);
    replenish_stock(&warehouse, item, 2);
    ioopm_add_to_cart(cart, item, 2);

    CU_ASSERT_PTR_NOT_NULL(cart);
    CU_ASSERT_TRUE(ioopm_is_in_shopping_cart(cart, item));
    CU_ASSERT_EQUAL(2, ioopm_amount_items_in_cart(cart, item));

    CU_ASSERT_TRUE(ioopm_remove_from_cart(cart, item));

    CU_ASSERT_FALSE(ioopm_is_in_shopping_cart(cart, item));
    CU_ASSERT_EQUAL(ioopm_amount_items_in_cart(cart, item), 0);

    CU_ASSERT_PTR_NOT_NULL(cart);

    ioopm_destroy_cart(cart);

    CU_ASSERT_PTR_NOT_NULL(cart);

    // ioopm_item_t *not_existing_item = make_item_backend("ape","a ape", 2);
    // CU_ASSERT_FALSE(ioopm_remove_from_cart(cart, not_existing_item));

    ioopm_destroy_item(item);
}

static void test_calculate_cost()
{
    ioopm_warehouse_t warehouse = ioopm_create_warehouse();
    ioopm_shopping_cart_t *cart = create_cart(100);
    ioopm_shopping_cart_t *cart_more = create_cart(1000);
    ioopm_item_t *item = make_item_backend(strdup("ape"), strdup("a ape"), 2);
    replenish_stock(&warehouse, item, 2);
    ioopm_add_to_cart(cart, item, 2);
    ioopm_add_to_cart(cart_more, item, 6);

    int cost = calculate_cost(cart);
    // int cost_more = calculate_cost(cart_more);
    CU_ASSERT_EQUAL(cost, 4);

    ioopm_shopping_cart_t *cart_is_empty = create_cart(100);

    int cost_empty = calculate_cost(cart_is_empty);
    CU_ASSERT_EQUAL(cost_empty, 0);

    ioopm_destroy_cart(cart);
    ioopm_destroy_cart(cart_more);
    ioopm_destroy_cart(cart_is_empty);
    ioopm_destroy_item(item);
}

static void test_checkout()
{
    ioopm_warehouse_t warehouse = ioopm_create_warehouse();
    ioopm_shopping_cart_t *cart_one = create_cart(100);
    ioopm_shopping_cart_t *cart_two = create_cart(100);
    ioopm_shopping_cart_t *cart_empty = create_cart(100);
    ioopm_item_t *item = make_item_backend(strdup("ape"), strdup("a ape"), 2);

    ioopm_add_item(warehouse.HTn, item);

    replenish_stock(&warehouse, item, 4);

    ioopm_add_to_cart(cart_one, item, 1);
    ioopm_add_to_cart(cart_two, item, 1);

    checkout(cart_one, warehouse.HTsl);
    checkout(cart_two, warehouse.HTsl);
    checkout(cart_empty, warehouse.HTsl);

    CU_ASSERT_EQUAL(2, ioopm_hash_table_size(warehouse.HTsl));

    ioopm_warehouse_destroy(&warehouse);
    ioopm_destroy_cart(cart_one);
    ioopm_destroy_cart(cart_two);
    ioopm_destroy_cart(cart_empty);
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
        (CU_add_test(my_test_suite, "Testing create", test_create) == NULL) ||
        (CU_add_test(my_test_suite, "Testing destroy", test_destroy) == NULL) ||
        (CU_add_test(my_test_suite, "Testing checkout", test_checkout) == NULL) ||
        (CU_add_test(my_test_suite, "Testing calculate cost", test_calculate_cost) == NULL) ||
        (CU_add_test(my_test_suite, "Testing remove cart items", test_remove_cart_items) == NULL) ||
        (CU_add_test(my_test_suite, "Testing add to cart", test_add_to_cart) == NULL) ||
        (CU_add_test(my_test_suite, "Testing choose cart", test_choose_cart) == NULL) ||
        (CU_add_test(my_test_suite, "Testing is in cart", test_is_in_shopping_cart) == NULL) ||
        (CU_add_test(my_test_suite, "Testing amounts items in cart", test_amount_items_in_cart) == NULL) ||
        (CU_add_test(my_test_suite, "Testing cart unique", test_cart_unique) == NULL) ||
        (CU_add_test(my_test_suite, "Testing id and eq cart", test_id_and_eq_cart) == NULL) ||
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
