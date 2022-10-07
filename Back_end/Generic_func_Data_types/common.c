#include "common.h"


//Compare elements func
bool ioopm_elem_is_empty(elem_t elem)
{
    return elem.f == 0 && elem.i == 0 && elem.u == 0 && elem.p == NULL && elem.s == NULL;
}

bool ioopm_elem_int_eq(elem_t elem1, elem_t elem2)
{
    return (elem1.i == elem2.i);
}

bool ioopm_elem_str_eq(elem_t elem1, elem_t elem2)
{
    char *str1 = elem1.s;
    char *str2 = elem2.s;
    return (strcmp(str1, str2) == 0);
}

bool ioopm_elem_bool_eq(elem_t elem1, elem_t elem2)
{
    if (elem1.b && elem2.b)
    {
        return true;
    }
    else
    {
        return (elem1.b == elem2.b);
    }
}

bool ioopm_elem_float_eq(elem_t elem1, elem_t elem2)
{
    return (elem1.f == elem2.f);
}

// For unsigned ints only
bool ioopm_elem_uint_eq(elem_t elem1, elem_t elem2)
{
    return (elem1.u == elem2.u);
}



//Hash func
int ioopm_string_hash(elem_t key)
{
    char *str = key.s;
    return (strlen(str) % no_buckets);
}

int ioopm_int_hash(elem_t key)
{
    int bucket = (key.i) % no_buckets;
    if (key.i < 0)
    {
        bucket += no_buckets;
    }
    return bucket;
}

int ioopm_uint_hash(elem_t key)
{
    int bucket = (key.u) % no_buckets;
    return bucket;
}



//to elem_t func
elem_t ioopm_int_to_elem(int i)
{
    elem_t elem = ioopm_create_empty_elem();
    elem.i = i;
    return elem;
}

elem_t ioopm_ptr_to_elem(void *ptr)
{
    elem_t elem = ioopm_create_empty_elem();
    elem.p = ptr;
    return elem;
}

elem_t ioopm_str_to_elem(char *str)
{
    elem_t elem = ioopm_create_empty_elem();
    elem.s = str;
    return elem;
}

elem_t ioopm_uint_to_elem(unsigned int uint)
{
    elem_t elem = ioopm_create_empty_elem();
    elem.u = uint;
    return elem;
}

elem_t ioopm_create_empty_elem()
{
    elem_t elem;
    elem.i = 0;
    elem.u = 0;
    elem.b = false;
    elem.f = 0;
    elem.s = NULL;
    elem.p = NULL;
    return elem;
}
