#include "store_specific_data_types.h"

bool ioopm_elem_item_eq(elem_t elem1, elem_t elem2)
{
   ioopm_item_t *item1 = elem1.p;
   ioopm_item_t *item2 = elem2.p;
   return (0==strcmp(*item1->name, *item2->name));
}