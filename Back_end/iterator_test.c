#include <CUnit/Basic.h>
#include "iterator.h"

static void test_next_has_next_current_reset(){
  //Variables
  bool is_true;
  bool is_true_other;
  elem_t abort_val = ioopm_create_empty_elem();
  elem_t val = ioopm_create_empty_elem();
  elem_t val_other = ioopm_create_empty_elem();

  //Create and fill list
  ioopm_list_t *ll = ioopm_linked_list_create(ioopm_elem_int_eq); 
  elem_t values[2] = {ioopm_int_to_elem(3),ioopm_int_to_elem(5)};
  ioopm_fill_linked_list(ll, values, 2);

  ioopm_list_t *empty_list = ioopm_linked_list_create(ioopm_elem_int_eq); 


  //Create iterator
  ioopm_list_iterator_t *iter = ioopm_list_iterator(ll);
  ioopm_list_iterator_t *empty = ioopm_list_iterator(empty_list);


  //Normal cases
  is_true = ioopm_iterator_has_next(iter);
  CU_ASSERT(is_true);

  val = ioopm_iterator_current(iter);
  val_other = ioopm_iterator_next(iter);
  CU_ASSERT(val.i != val_other.i);

  ioopm_iterator_reset(iter); // reset the position
  val = ioopm_iterator_next(iter);
  ioopm_iterator_reset(iter);
  val_other = ioopm_iterator_next(iter);
  CU_ASSERT(val.i == val_other.i);


  //Special cases
    //NULL and empty iterator
  val = ioopm_iterator_next(NULL);
  val_other = ioopm_iterator_next(empty);
  CU_ASSERT((val.i == abort_val.i) && (val_other.i == abort_val.i));

  is_true = ioopm_iterator_has_next(NULL);
  is_true_other = ioopm_iterator_has_next(empty);
  CU_ASSERT((!is_true && !is_true_other));

  val = ioopm_iterator_current(NULL);
  val_other = ioopm_iterator_current(empty);
  CU_ASSERT((val.i == abort_val.i) && (val_other.i == abort_val.i));

  ioopm_iterator_reset(NULL);
  ioopm_iterator_reset(empty);

  //iterator is at end of list
  ioopm_iterator_reset(iter);
  ioopm_iterator_next(iter);
  ioopm_iterator_next(iter);
  val = ioopm_iterator_next(iter);
  CU_ASSERT(val.i == abort_val.i);
  

  //Destroy
  ioopm_linked_list_destroy(ll);
  ioopm_linked_list_destroy(empty_list);
  ioopm_iterator_destroy(iter);
  ioopm_iterator_destroy(empty);
}

static void test_insert_remove(){
    //Create
    ioopm_list_t *ll = ioopm_linked_list_create(ioopm_elem_int_eq); 
    ioopm_list_iterator_t *iter = ioopm_list_iterator(ll);

    //empty things and null
    // CU_ASSERT((iter) == -1);
    // CU_ASSERT(ioopm_iteratorioopm_iterator_remove_remove(NULL) == -1);
    ioopm_iterator_insert(NULL, ioopm_int_to_elem(5));
    ioopm_iterator_insert(iter, ioopm_int_to_elem(9)); //should error because current is NULL (actually, no, will insert an element as list head because list head is also NULL)

    //inserting, checking, removing an element
    ioopm_iterator_insert(iter, ioopm_int_to_elem(5));
    CU_ASSERT(ioopm_linked_list_contains(ll, ioopm_int_to_elem(5)));
    // CU_ASSERT(ioopm_iterator_remove(iter) == 5); //remove is CANCELLED
    CU_ASSERT(!ioopm_linked_list_is_empty(ll)); 
    
    elem_t values[5] = {ioopm_int_to_elem(4),ioopm_int_to_elem(6),ioopm_int_to_elem(7),ioopm_int_to_elem(3),ioopm_int_to_elem(-1)};
    ioopm_fill_linked_list(ll, values, 5);
    ioopm_iterator_reset(iter);
    ioopm_iterator_next(iter);
    ioopm_iterator_next(iter);
    ioopm_iterator_insert(iter, ioopm_int_to_elem(19)); //should put 19 in index 2 of list
    // CU_ASSERT(ioopm_linked_list_get(ll, 2) == 19);
    CU_ASSERT((ioopm_iterator_current(iter)).i == 19); //which should be current also
    // ioopm_iterator_next(iter);
    // CU_ASSERT(ioopm_iterator_remove(iter) == 7); //and the next should be 7 which is now removed
    ioopm_iterator_reset(iter);
    ioopm_iterator_insert(iter, ioopm_int_to_elem(199));
    // CU_ASSERT(ioopm_iterator_remove(iter) == 4); //removing head
    CU_ASSERT((ioopm_linked_list_get(ll, 0)).i == 199); //check that head was correctly updated
    

    //Destroy
    ioopm_linked_list_destroy(ll);
    ioopm_iterator_destroy(iter);
}



static void test_destroy(){
  //Create and fill list
  ioopm_list_t *ll = ioopm_linked_list_create(ioopm_elem_int_eq); 
  elem_t values[2] = {ioopm_int_to_elem(3),ioopm_int_to_elem(5)};
  ioopm_fill_linked_list(ll, values, 2);

  ioopm_list_t *empty_list = ioopm_linked_list_create(ioopm_elem_int_eq); 


  //Create iterator
  ioopm_list_iterator_t *iter = ioopm_list_iterator(ll);
  ioopm_list_iterator_t *empty = ioopm_list_iterator(empty_list);

  //Normal cases
  ioopm_iterator_destroy(iter);

  //Special cases
    //NULL and empty iterator
  ioopm_iterator_destroy(NULL);
  ioopm_iterator_destroy(empty);

  
  //Destroy
  ioopm_linked_list_destroy(ll);
  ioopm_linked_list_destroy(empty_list);
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
      (CU_add_test(my_test_suite, "Basic next, has next, current and reset", test_next_has_next_current_reset) == NULL) ||
      (CU_add_test(my_test_suite, "Basic insert and remove", test_insert_remove) == NULL) ||
      (CU_add_test(my_test_suite, "Basic destroy", test_destroy) == NULL) ||
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
