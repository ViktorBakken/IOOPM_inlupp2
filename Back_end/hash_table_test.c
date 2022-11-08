//libraries
#include <CUnit/Basic.h>
#include "hash_table.h"

//defined macros
#define ioopm_int_str_ht_insert(ht, i, s) \
  ioopm_hash_table_insert(ht, ioopm_int_to_elem(i), ioopm_str_to_elem(s))

#define ioopm_has_str_ht_value(ht, s) \
  ioopm_hash_table_has_value(ht, ioopm_str_to_elem(s))

#define ioopm_ht_int_lookup(ht, k) \
  ioopm_hash_table_lookup(ht, ioopm_int_to_elem(k))

#define ioopm_ht_int_remove(ht, k) \
  ioopm_hash_table_remove(ht, ioopm_int_to_elem(k))


//functions to test ioopm_hash_table_any / ioopm_hash_table_all / ioopm_hash_table_apply_to_all
bool key_int_equiv(elem_t key, elem_t value_ignored, void *x)
{
  (void) value_ignored;
  int *int_ptr = x;
  int i = *int_ptr;

  elem_t elem_int = ioopm_int_to_elem(i);
  return ioopm_elem_int_eq(key, elem_int);
}

bool value_str_equiv(elem_t key_ignored, elem_t value, void *x)
{
  (void) key_ignored;
  char *value_str = value.s;
  char *other_value_ptr = x;
  return (0 == strcmp(value_str, other_value_ptr));
}

static void fun_string_changer(elem_t value_ignored, elem_t *value, void *extra)
{
  (void) value_ignored;
  char *other_value_ptr = extra;
  value->p = other_value_ptr;
}

//ease of life function
static void fill_hash_table(ioopm_hash_table_t *ht, elem_t *keys, elem_t *values, size_t length)
{ // insert values into hash table
  if (keys && values)
  {
    for (size_t i = 0; i < length; i++)
    {
      ioopm_hash_table_insert(ht, keys[i], values[i]);
    }
  }
}


///TESTS
static void test_hash_table_all()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(ioopm_elem_int_eq, ioopm_elem_str_eq, NULL);
  elem_t empty = ioopm_create_empty_elem();
  elem_t values_elem[10];
  for (size_t i = 0; i < 10; i++)
  {
    values_elem[i] = ioopm_create_empty_elem(); // cleans up the elem_t
    empty.s = "hej";
    values_elem[i] = empty;
  }

  elem_t keys_elem[10] = {ioopm_int_to_elem(3), ioopm_int_to_elem(5), ioopm_int_to_elem(100), ioopm_int_to_elem(35), ioopm_int_to_elem(4), ioopm_int_to_elem(5), ioopm_int_to_elem(-7), ioopm_int_to_elem(8), ioopm_int_to_elem(9), ioopm_int_to_elem(11)};

  fill_hash_table(ht, keys_elem, values_elem, 10);

  CU_ASSERT(ioopm_hash_table_all(ht, value_str_equiv, &"hej"));
  ioopm_int_str_ht_insert(ht, 13, "inte h"); // lägger till en annan sträng och testar att det blir falskt
  CU_ASSERT(!ioopm_hash_table_all(ht, value_str_equiv, &"hej"));
  ioopm_hash_table_destroy(ht);
}

static void test_hash_table_has_value()
{
  // create hashtable and value, and insert t value
  ioopm_hash_table_t *ht = ioopm_hash_table_create(ioopm_elem_int_eq, ioopm_elem_str_eq, NULL);
  char *str1 = calloc(9, sizeof(char));
  strcpy(str1, "Varmkorv");
  ioopm_int_str_ht_insert(ht, 4, str1);

  // test 1 (equivalent string)
  CU_ASSERT(ioopm_has_str_ht_value(ht, str1));
  // test 2 (string of same value)
  CU_ASSERT(ioopm_has_str_ht_value(ht, "Varmkorv"));
  // test 3 (string that doesn't exist)
  CU_ASSERT(!ioopm_has_str_ht_value(ht, "rctvybuybvt"));

  free(str1);
  ioopm_hash_table_destroy(ht);
}

static void test_hash_table_has_key()
{
  int stop_test_num = 28;
  int start_test_num = -3;
  ioopm_hash_table_t *ht = ioopm_hash_table_create(ioopm_elem_int_eq, ioopm_elem_str_eq, NULL);

  // start of test
  // has non existing key
  elem_t a = ioopm_int_to_elem(-1);
  CU_ASSERT(!ioopm_hash_table_has_key(ht, a)); // search for key -1 in bucket 16.
  CU_ASSERT(!ioopm_hash_table_any(ht, key_int_equiv, &a));

  for (int i = start_test_num; i < stop_test_num; i++) // fill hash_table
  {
    ioopm_int_str_ht_insert(ht, i, NULL); // insert from negative num to positiv
  }

  elem_t key = ioopm_create_empty_elem();
  for (int j = start_test_num; j < stop_test_num; j++) // look up test
  {
    key = ioopm_int_to_elem(j);
    CU_ASSERT(ioopm_hash_table_has_key(ht, ioopm_int_to_elem(j)));
    CU_ASSERT(ioopm_hash_table_any(ht, key_int_equiv, &key));
  }
  // end of test

  ioopm_hash_table_destroy(ht);
}
static void test_hash_table_keys()
{
  // define a key array and bool array
  elem_t values_elem[10] = {ioopm_str_to_elem("tre"), ioopm_str_to_elem("fem ett"), ioopm_str_to_elem("hundra"), ioopm_str_to_elem("trettiofem"), ioopm_str_to_elem("fyra"), ioopm_str_to_elem("fem II"), ioopm_str_to_elem("minus sju"), ioopm_str_to_elem("åtta"), ioopm_str_to_elem("nio"), ioopm_str_to_elem("elva")};
  elem_t keys[10] = {ioopm_int_to_elem(3), ioopm_int_to_elem(5), ioopm_int_to_elem(100), ioopm_int_to_elem(35), ioopm_int_to_elem(4), ioopm_int_to_elem(-9), ioopm_int_to_elem(-7), ioopm_int_to_elem(8), ioopm_int_to_elem(9), ioopm_int_to_elem(11)};
  bool found[10] = {false};

  ioopm_hash_table_t *ht = ioopm_hash_table_create(ioopm_elem_int_eq, ioopm_elem_str_eq, NULL);

  fill_hash_table(ht, keys, values_elem, 10);

  ioopm_list_t *keys_list = ioopm_hash_table_keys(ht);
  elem_t current_key;
  bool is_found = false;

  for (size_t i = 0; i < 10; i++)
  {
    current_key = ioopm_linked_list_get(keys_list, i);

    for (size_t j = 0; (j < 10) && !is_found; j++) // Checks to se if current_key is in keys[]
    {
      if (ht->key_eq_fn(current_key, keys[j])) // current key is in keys[] => it is marked in found[], and is_found = true
      {
        found[j] = true;
        is_found = true;
      }
    }
    if (!is_found) // current_key is not in keys[]
    {
      CU_FAIL("Found a key that was never inserted!");
    }
    else // reset is_found!
    {
      is_found = false;
    }
  }

  for (size_t i = 0; i < 10; i++) // tests if all the given keys are found!
  {
    CU_ASSERT(found[i]);
  }

  ioopm_linked_list_destroy(keys_list);
  ioopm_hash_table_destroy(ht);
}

static void test_hash_table_values()
{
  elem_t values_elem[10] = {ioopm_str_to_elem("tre"), ioopm_str_to_elem("fem ett"), ioopm_str_to_elem("hundra"), ioopm_str_to_elem("trettiofem"), ioopm_str_to_elem("fyra"), ioopm_str_to_elem("fem II"), ioopm_str_to_elem("minus sju"), ioopm_str_to_elem("åtta"), ioopm_str_to_elem("nio"), ioopm_str_to_elem("elva")};
  elem_t keys[10] = {ioopm_int_to_elem(3), ioopm_int_to_elem(-25), ioopm_int_to_elem(100), ioopm_int_to_elem(35), ioopm_int_to_elem(4), ioopm_int_to_elem(5), ioopm_int_to_elem(-7), ioopm_int_to_elem(8), ioopm_int_to_elem(9), ioopm_int_to_elem(11)};
  ioopm_hash_table_t *ht = ioopm_hash_table_create(ioopm_elem_int_eq, ioopm_elem_str_eq, NULL);

  fill_hash_table(ht, keys, values_elem, 10);

  ioopm_list_t *new_keys = ioopm_hash_table_keys(ht);
  ioopm_list_t *new_values = ioopm_hash_table_values(ht);

  bool is_found = false;
  elem_t old_key;
  elem_t new_key;
  elem_t new_value;
  for (size_t i = 0; i < 10; i++) // dessa for-loopar: för varje nyckel i originallistan hittar den sin motsvariga i nya listan.
  {
    old_key = keys[i];
    for (size_t n = 0; n < 10 && !is_found; n++)
    {
      new_key = ioopm_linked_list_get(new_keys, n);
      is_found = false;
      if (ht->key_eq_fn(old_key, new_key)) // ifall de hitter motsvariga nycklar så jämförs dess strängar
      {
        is_found = true;
        new_value = ioopm_linked_list_get(new_values, n);
        CU_ASSERT(ht->value_eq_fn(values_elem[i], new_value));
      }
    }
    if (is_found == false)
    {
      CU_FAIL("Found a key that was never inserted!")
    }
    is_found = false;
  }

  ioopm_hash_table_apply_to_all(ht, fun_string_changer, "h"); // vi nu ändrar vi alla values till "h"
  ioopm_list_t *ht_values = ioopm_hash_table_values(ht);      // skapar en ny array med de nya värderna!

  for (size_t i = 0; i < 10; i++) // dessa for-loopar: för varje nyckel i originallistan hittar den sin motsvariga i nya listan.
  {
    old_key = keys[i];
    for (size_t n = 0; n < 10 && !is_found; n++)
    {
      is_found = false;
      new_key = ioopm_linked_list_get(new_keys, n);
      if (ht->key_eq_fn(old_key, new_key)) // ifall de hitter motsvariga nycklar så jämförs dess strängar
      {
        is_found = true;
        new_value = ioopm_linked_list_get(ht_values, i);
        CU_ASSERT(ht->value_eq_fn(new_value, ioopm_str_to_elem("h"))); // testar ioopm_hash_table_apply_to_all()!!!
      }
    }
    if (is_found == false)
    {
      CU_FAIL("Found a key that was never inserted!")
    }
    is_found = false;
  }

  ioopm_hash_table_destroy(ht);
  ioopm_linked_list_destroy(new_keys);

  ioopm_linked_list_destroy(new_values);
  ioopm_linked_list_destroy(ht_values);
}

static void test_values_keys()
{
  test_hash_table_values();
  test_hash_table_keys();
}

static void test_ht_size_and_empty()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(ioopm_elem_int_eq, ioopm_elem_str_eq, NULL);
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 0);
  CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht));
  ioopm_int_str_ht_insert(ht, 1, "majs");
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 1);
  ioopm_int_str_ht_insert(ht, 7, "jajaj");
  ioopm_int_str_ht_insert(ht, 5, "agagag");
  ioopm_int_str_ht_insert(ht, 11, "afeafe");
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 4);
  CU_ASSERT_FALSE(ioopm_hash_table_is_empty(ht));
  ioopm_hash_table_destroy(ht);
}

static void test_clear()
{
  ioopm_hash_table_t *empty = ioopm_hash_table_create(ioopm_elem_int_eq, ioopm_elem_str_eq, NULL);
  ioopm_hash_table_t *ht = ioopm_hash_table_create(ioopm_elem_int_eq, ioopm_elem_str_eq, NULL);
  CU_ASSERT(ioopm_hash_table_is_empty(ht));
  CU_ASSERT(ioopm_hash_table_is_empty(empty));

  ioopm_int_str_ht_insert(ht, 16, "ello");
  ioopm_int_str_ht_insert(ht, 16, "dello"); // is ignored
  ioopm_int_str_ht_insert(ht, 1, "gaogd");
  ioopm_int_str_ht_insert(ht, 7, "ello");

  ioopm_hash_table_clear(ht);
  CU_ASSERT(ioopm_hash_table_is_empty(ht) && ioopm_hash_table_is_empty(empty));
  ioopm_hash_table_destroy(empty);
  ioopm_hash_table_destroy(ht);
}

static void test_create_destroy()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(ioopm_elem_int_eq, ioopm_elem_str_eq, NULL);
  CU_ASSERT_PTR_NOT_NULL(ht);
  ioopm_hash_table_destroy(ht);
}

static void test_insert_once(ioopm_hash_table_t *ht)
{ // Lookup empty ht
  elem_t elem = ioopm_ht_int_lookup(ht, 1).value;
  bool is_true = ioopm_elem_is_empty(elem);
  CU_ASSERT(is_true);

  ioopm_int_str_ht_insert(ht, 1, "majs");

  ioopm_option_t tmp = ioopm_ht_int_lookup(ht, 1);
  if (tmp.success) // test lookup insert once
  {
    CU_ASSERT(ht->value_eq_fn(tmp.value, ioopm_str_to_elem("majs")));
  }
}

static void test_remove(ioopm_hash_table_t *ht)
{
  elem_t elem;
  bool is_true;

  ioopm_int_str_ht_insert(ht, 98, "vajs");
  ioopm_int_str_ht_insert(ht, -4, "bajs");

  ioopm_option_t tmp = ioopm_ht_int_remove(ht, 98); // remove

  if (tmp.success)
  {
    CU_ASSERT(ht->value_eq_fn(tmp.value, ioopm_str_to_elem("vajs"))); // checked if we removed the wanted value

    elem = ioopm_ht_int_lookup(ht, 98).value; // look up removed value
    is_true = ioopm_elem_is_empty(elem);
    CU_ASSERT(is_true);
  }

  ioopm_option_t tmp2 = ioopm_ht_int_remove(ht, 0); // remove unexisting value
  is_true = ioopm_elem_is_empty(tmp2.value);
  CU_ASSERT(is_true);
}

static void test_lookup_empty(ioopm_hash_table_t *ht)
{
  elem_t elem;
  bool is_true;
  for (int i = 0; i < no_buckets; ++i) // no_buckets is a non-magical number!! Completly mundane!
  {
    elem = ioopm_ht_int_lookup(ht, i).value;
    is_true = ioopm_elem_is_empty(elem);
    CU_ASSERT(is_true);
  }
  CU_ASSERT(!ioopm_ht_int_lookup(ht, -1).success);
}

static void test_insert_lookup()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(ioopm_elem_int_eq, ioopm_elem_str_eq, ioopm_int_hash);
  test_lookup_empty(ht);
  test_insert_once(ht);
  test_remove(ht);
  ioopm_hash_table_destroy(ht);
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
      (CU_add_test(my_test_suite, "Create and des", test_create_destroy) == NULL) ||
      (CU_add_test(my_test_suite, "Insert, lookup test and remove", test_insert_lookup) == NULL) ||
      (CU_add_test(my_test_suite, "hash_table_size() test", test_ht_size_and_empty) == NULL) ||
      (CU_add_test(my_test_suite, "ht clear is_empty test", test_clear) == NULL) ||
      (CU_add_test(my_test_suite, "test insertion and look up of values/keys", test_values_keys) == NULL) ||
      (CU_add_test(my_test_suite, "tests hash_table_has_key function", test_hash_table_has_key) == NULL) ||
      (CU_add_test(my_test_suite, "tests hash_table_has_values function", test_hash_table_has_value) == NULL) ||
      (CU_add_test(my_test_suite, "tests hash_table_all function", test_hash_table_all) == NULL) ||
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
