#include <CUnit/Basic.h>
#include "linked_list.h"



//make all values to the link's index + the extra value (for testing apply_to_all)
static void value_to_index(size_t index, elem_t *value, void *extra) 
{
  int *extr = extra;
  value->i = (int) index + *extr;
}

//check is value is equal to extra (for testing list_any and list_all)
static bool value_is_extra(size_t index_ignored, elem_t value, void *extra)
{
  if (extra)
  {
    int *val2 = extra;
    return (value.i == *val2);
  }
  else {return false;}
  
}


static void test_apply_to_all()
{
  //create list and fill
  ioopm_list_t *list = ioopm_linked_list_create(ioopm_elem_int_eq);
  elem_t values[10] = {ioopm_int_to_elem(3),ioopm_int_to_elem(5),ioopm_int_to_elem(100),ioopm_int_to_elem(35),ioopm_int_to_elem(4),ioopm_int_to_elem(5),ioopm_int_to_elem(-7),ioopm_int_to_elem(8),ioopm_int_to_elem(9),ioopm_int_to_elem(11)};
  ioopm_fill_linked_list(list, values, 10);
  //do the deed
  int extra = 2;
  ioopm_linked_list_apply_to_all(list, value_to_index, &extra);
  //test if they are correct
  elem_t test_elem = ioopm_create_empty_elem();
  for (int i = 0; i < 10; i++)
  {
    test_elem = ioopm_linked_list_get(list, i);
    CU_ASSERT((i + 2) == test_elem.i);
  }
  //null list case
  ioopm_list_t *list2 = ioopm_linked_list_create(ioopm_elem_int_eq);
  ioopm_linked_list_apply_to_all(list2, value_to_index, &extra);
  ioopm_linked_list_apply_to_all(list2, value_to_index, NULL);
  ioopm_linked_list_apply_to_all(NULL, value_to_index, &extra);
  ioopm_linked_list_apply_to_all(NULL, value_to_index, NULL);
  //cleanup
  ioopm_linked_list_destroy(list);
  ioopm_linked_list_destroy(list2);
}

static void test_list_any_all()
{
  //create and fill list
  ioopm_list_t *list = ioopm_linked_list_create(ioopm_elem_int_eq);
  ioopm_list_t *empty = ioopm_linked_list_create(ioopm_elem_int_eq);
  elem_t values[10] = {ioopm_int_to_elem(3),ioopm_int_to_elem(5),ioopm_int_to_elem(100),ioopm_int_to_elem(35),ioopm_int_to_elem(4),ioopm_int_to_elem(5),ioopm_int_to_elem(-7),ioopm_int_to_elem(8),ioopm_int_to_elem(9),ioopm_int_to_elem(11)};
  ioopm_fill_linked_list(list, values, 10);

  //Special cases
    //NULL and empty list
    bool is_empty = ioopm_linked_list_any(empty,value_is_extra,&values[0]);
    bool is_NULL = ioopm_linked_list_any(NULL,value_is_extra,&values[0]);
    CU_ASSERT(!is_empty && !is_NULL);

    is_empty = ioopm_linked_list_all(empty,value_is_extra,&values[0]);
    is_NULL = ioopm_linked_list_all(NULL,value_is_extra,&values[0]);
    CU_ASSERT(!is_empty && !is_NULL);

  //do the thing
  int arg1 = 35;
  int arg2 = -7;
  int arg3 = 101;
  int arg4 = 0;
  CU_ASSERT(ioopm_linked_list_any(list, value_is_extra, &arg1));
  CU_ASSERT(ioopm_linked_list_any(list, value_is_extra, &arg2));
  CU_ASSERT(!ioopm_linked_list_any(list, value_is_extra, &arg3));
  CU_ASSERT(!ioopm_linked_list_any(list, value_is_extra, &arg4));
  ioopm_linked_list_remove(list, 3);
  CU_ASSERT(!ioopm_linked_list_any(list, value_is_extra, &arg1));
  //now again but for list_all
  ioopm_list_t *list2 = ioopm_linked_list_create(ioopm_elem_int_eq);
  elem_t values2[5] = {ioopm_int_to_elem(1),ioopm_int_to_elem(1),ioopm_int_to_elem(1),ioopm_int_to_elem(1),ioopm_int_to_elem(1)};
  ioopm_fill_linked_list(list2, values2, 5);
  int arg5 = 1;
  CU_ASSERT(ioopm_linked_list_all(list2, value_is_extra, &arg5));
  CU_ASSERT(!ioopm_linked_list_all(list2, value_is_extra, &arg2));
  CU_ASSERT(!ioopm_linked_list_all(list, value_is_extra, &arg5));
  ioopm_linked_list_insert(list2, 4, ioopm_int_to_elem(-77));
  CU_ASSERT(!ioopm_linked_list_all(list2, value_is_extra, &arg5));
  //empty list case and null extra and null list
  ioopm_list_t *list3 = ioopm_linked_list_create(ioopm_elem_int_eq);
  CU_ASSERT(!ioopm_linked_list_all(list3, value_is_extra, &arg5));
  CU_ASSERT(!ioopm_linked_list_all(list3, value_is_extra, NULL));
  CU_ASSERT(!ioopm_linked_list_all(list2, value_is_extra, NULL));
  CU_ASSERT(!ioopm_linked_list_all(NULL, value_is_extra, NULL));
  CU_ASSERT(!ioopm_linked_list_all(list2, value_is_extra, &arg5));

  CU_ASSERT(!ioopm_linked_list_all(list3, value_is_extra, &arg5));
  CU_ASSERT(!ioopm_linked_list_all(list3, value_is_extra, NULL));
  CU_ASSERT(!ioopm_linked_list_all(list2, value_is_extra, NULL));
  CU_ASSERT(!ioopm_linked_list_all(NULL, value_is_extra, NULL));
  CU_ASSERT(!ioopm_linked_list_all(list2, value_is_extra, &arg5));
  ioopm_linked_list_destroy(list);
  ioopm_linked_list_destroy(list2);
  ioopm_linked_list_destroy(list3);
  ioopm_linked_list_destroy(empty);
}


static void test_create_destroy_insert_append_prepend()
{
  int positiv = 10;
  int negativ = -10;
  int old_size;
  int new_size;
  int end_of_list;

  ioopm_list_t *ll = ioopm_linked_list_create(ioopm_elem_int_eq);

  // test negativ input
  ioopm_linked_list_append(ll, ioopm_int_to_elem(negativ));
  CU_ASSERT((ioopm_linked_list_get(ll, 0)).i == negativ);

  ioopm_linked_list_prepend(ll, ioopm_int_to_elem(negativ - 5));
  CU_ASSERT((ioopm_linked_list_get(ll, 0)).i == negativ - 5);

  ioopm_linked_list_insert(ll, 1, ioopm_int_to_elem(negativ + 1));
  CU_ASSERT((ioopm_linked_list_get(ll, 1)).i == negativ + 1);

  // test Normal input
  end_of_list = ioopm_linked_list_size(ll);

  ioopm_linked_list_append(ll, ioopm_int_to_elem(positiv));
  CU_ASSERT((ioopm_linked_list_get(ll, end_of_list)).i == positiv);

  ioopm_linked_list_prepend(ll, ioopm_int_to_elem(positiv - 5));

  ioopm_linked_list_insert(ll, 1, ioopm_int_to_elem(positiv + 1));
  CU_ASSERT((ioopm_linked_list_get(ll, 1)).i == positiv + 1);

  ioopm_linked_list_insert(ll, 0, ioopm_int_to_elem(positiv + 51));
  CU_ASSERT((ioopm_linked_list_get(ll, 0)).i == positiv + 51);
  // test special cases
    // negative index insert
  old_size = ioopm_linked_list_size(ll);
  ioopm_linked_list_insert(ll, negativ, ioopm_int_to_elem(7));
  new_size = ioopm_linked_list_size(ll);
  CU_ASSERT(old_size == new_size);

  //   // out-of bounds index
  old_size = ioopm_linked_list_size(ll);
  ioopm_linked_list_insert(ll, 100, ioopm_int_to_elem(7));
  new_size = ioopm_linked_list_size(ll);
  CU_ASSERT(old_size == new_size);


    // insert in NULL List
  ioopm_linked_list_append(NULL, ioopm_int_to_elem(1));
  ioopm_linked_list_prepend(NULL, ioopm_int_to_elem(0));
  ioopm_linked_list_insert(NULL, 1, ioopm_int_to_elem(7));

    // destroy NULL list
  ioopm_linked_list_destroy(NULL);

  // destroy lists
  ioopm_linked_list_destroy(ll);
}

static void test_size_empty()
{
  // Change test variables
  int insert_size = 10;
  elem_t values[10] = {ioopm_int_to_elem(1), ioopm_int_to_elem(2), ioopm_int_to_elem(3), ioopm_int_to_elem(5), ioopm_int_to_elem(4), ioopm_int_to_elem(3), ioopm_int_to_elem(8), ioopm_int_to_elem(7), ioopm_int_to_elem(10), ioopm_int_to_elem(8000)};

  // Test variables
  int empty_int = 0;
  ioopm_list_t *empty = ioopm_linked_list_create(ioopm_elem_int_eq);
  int size;
  int size_other;
  bool is_empty;

  // Create and fill linked list
  ioopm_list_t *ll = ioopm_linked_list_create(ioopm_elem_int_eq);
  ioopm_fill_linked_list(ll, values, insert_size);

  // Normal Cases
  size = ioopm_linked_list_size(ll);
  CU_ASSERT(size == insert_size);

  is_empty = ioopm_linked_list_is_empty(ll); // Checks if filled linked list is empty
  CU_ASSERT(!is_empty == ioopm_linked_list_is_empty(empty));


  // Special Cases
  //  NULL and empty linked list
  size = ioopm_linked_list_size(NULL);
  size_other = ioopm_linked_list_size(empty);
  CU_ASSERT((size == empty_int) && (size_other == empty_int));

  is_empty = ioopm_linked_list_is_empty(NULL);
  CU_ASSERT(is_empty && ioopm_linked_list_is_empty(empty));

  // Destroy lists
   ioopm_linked_list_destroy(ll);
   ioopm_linked_list_destroy(empty);
}

static void test_remove_clear()
{
  // Change test variables
  int insert_size = 10;
  elem_t values[10] = {ioopm_int_to_elem(1), ioopm_int_to_elem(2), ioopm_int_to_elem(3), ioopm_int_to_elem(5), ioopm_int_to_elem(4), ioopm_int_to_elem(3), ioopm_int_to_elem(8), ioopm_int_to_elem(7), ioopm_int_to_elem(10), ioopm_int_to_elem(8000)};

  // Test variables
  ioopm_list_t *empty = ioopm_linked_list_create(ioopm_elem_int_eq);
  elem_t abort_val = ioopm_create_empty_elem();
  elem_t val = ioopm_create_empty_elem();
  elem_t val_other = ioopm_create_empty_elem();
  int size;

  // Create and fill linked list
  ioopm_list_t *ll = ioopm_linked_list_create(ioopm_elem_int_eq);
  ioopm_fill_linked_list(ll, values, insert_size);

  // Normal cases
  val = ioopm_linked_list_get(ll, 1);
  CU_ASSERT(val.i == values[1].i);

  val_other = ioopm_linked_list_remove(ll, 1);
  CU_ASSERT(val.i == val_other.i);

  val_other = ioopm_linked_list_remove(ll, 0);
  CU_ASSERT(values[0].i == val_other.i);

  ioopm_linked_list_clear(ll);
  size = ioopm_linked_list_size(ll);
  CU_ASSERT(size == 0);


  // Special cases
    // negative index remove
  val = ioopm_linked_list_remove(ll, -5);
  CU_ASSERT(val.i == abort_val.i);


    // index out of bounds
  val = ioopm_linked_list_remove(ll, 100);
  CU_ASSERT(val.i == abort_val.i);

    // NULL and empty linked list
  val = ioopm_linked_list_remove(NULL, 1);
  val_other = ioopm_linked_list_remove(empty, 1);
  CU_ASSERT((val.i == abort_val.i) && (val_other.i == abort_val.i));
  
  ioopm_linked_list_clear(NULL);
  ioopm_linked_list_clear(empty);


    //Remove empty
  val = ioopm_linked_list_remove(empty, 0);
  CU_ASSERT(val.i == abort_val.i);


  // Destroy lists
  ioopm_linked_list_destroy(empty);
  ioopm_linked_list_destroy(ll);
}

static void test_get_containts()
{
  // Change test variables
  int insert_size = 10;
  elem_t values[10] = {ioopm_int_to_elem(1), ioopm_int_to_elem(2), ioopm_int_to_elem(3), ioopm_int_to_elem(5), ioopm_int_to_elem(4), ioopm_int_to_elem(3), ioopm_int_to_elem(8), ioopm_int_to_elem(7), ioopm_int_to_elem(10), ioopm_int_to_elem(8000)};

  // Test variables
  bool is_true;
  elem_t err_num = ioopm_create_empty_elem();
  elem_t val = ioopm_create_empty_elem();

  // Create and fill linked list
  ioopm_list_t *ll = ioopm_linked_list_create(ioopm_elem_int_eq);
  ioopm_fill_linked_list(ll, values, insert_size);

  // Normal cases
  for (int i = 0; i < insert_size; i++)
  {
    val = ioopm_linked_list_get(ll, i);
    is_true = ioopm_linked_list_contains(ll,values[i]);
    
    CU_ASSERT(is_true);
    CU_ASSERT(val.i == (values[i]).i);
  }

  // Special cases
    // negative index
  val = ioopm_linked_list_get(ll, -1);
  CU_ASSERT(val.i == err_num.i);

  is_true = ioopm_linked_list_contains(ll, ioopm_int_to_elem(-1));
  CU_ASSERT(!is_true);

    //out of bounds
  val = ioopm_linked_list_get(ll, 100);
  CU_ASSERT(val.i == err_num.i);

  is_true = ioopm_linked_list_contains(ll, ioopm_int_to_elem(100));
  CU_ASSERT(!is_true);

    //NULL link
  val = ioopm_linked_list_get(NULL, 100);
  CU_ASSERT(val.i == err_num.i);

  is_true = ioopm_linked_list_contains(NULL, ioopm_int_to_elem(100));
  CU_ASSERT(!is_true);

  //Destroy lists
  ioopm_linked_list_destroy(ll);
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
      (CU_add_test(my_test_suite, "Basic insert and destroy tests", test_create_destroy_insert_append_prepend) == NULL) ||
      (CU_add_test(my_test_suite, "Basic size and empty tests", test_size_empty) == NULL) ||
      (CU_add_test(my_test_suite, "Basic remove and clear", test_remove_clear) == NULL) ||
      (CU_add_test(my_test_suite, "Basic get and contains", test_get_containts) == NULL) ||
      (CU_add_test(my_test_suite, "Basic apply_to_all test", test_apply_to_all) == NULL) ||
      (CU_add_test(my_test_suite, "Basic any/all test", test_list_any_all) == NULL) ||
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
