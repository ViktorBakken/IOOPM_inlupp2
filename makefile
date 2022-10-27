cc = gcc
flag = -g -Wall -Wextra
flagExtra = --coverage -pg
CUnit = -lcunit

clr_flags = -k -s -i

# Which directory is the file stored at
store_plac = store.c
db_plac = db.c
back_end_plac = Back_end/back_end.c 
iterator_plac = Back_end/iterator.c 
hash_table_plac = Back_end/hash_table.c 
linked_list_plac = Back_end/linked_list.c
shopping_cart_plac = Back_end/shopping_cart.c  
common_plac = Back_end/Generic_func_Data_types/common.c
utils_plac = Back_end/Generic_func_Data_types/utils.c
store_specific_data_types_plac = Back_end/Generic_func_Data_types/store_specific_data_types.c 


#The dependecies of each file
db_depend = $(db_plac) $(back_end_plac) $(utils_plac) $(store_specific_data_types_plac)
back_end_depend = $(back_end_plac) $(hash_table_plac) $(linked_list_plac) $(iterator_plac) $(common_plac) $(store_specific_data_types_plac)
iterator_depend = $(iterator_plac) $(common_plac)
hash_table_depend =$(hash_table_plac) $(linked_list_plac) $(common_plac)
linked_list_depend = $(linked_list_plac) $(iterator_plac) $(common_plac)
shopping_cart_depend = $(back_end_plac) $(store_specific_data_types_plac) $(db_plac)
utils_depend = $(utils_plac)
common_depend = $(common_plac)
store_specific_data_types_depend = $(store_specific_data_types_plac)

#Dependencies for store.c and their .o files
object.c = $(store_plac) $(db_plac) $(back_end_plac)  $(iterator_plac) $(hash_table_plac) $(linked_list_plac)  $(shopping_cart_plac)   $(common_plac) $(utils_plac) $(store_specific_data_types_plac)
# object.h = store.h db.h    Back_end/back_end.h  Back_end/iterator.h Back_end/hash_table.h Back_end/linked_list.h  Back_end/shopping_cart.h   Back_end/Generic_func_Data_types/common.h Back_end/Generic_func_Data_types/utils.h Back_end/Generic_func_Data_types/store_specific_data_types.h
object.o = store.o db.o back_end.o iterator.o hash_table.o linked_list.o shopping_cart.o common.o utils.o store_specific_data_types.o
object_test = store_test.c


# %_test.o : $(object.c)ob
# 	$(cc) $^ $(flag) object_test.c $(CUnit) -c


store: $(object.o)
	$(cc) $^ $(flag) -o store

store.o : $(object.c)
	$(cc) $^ $(flag) -c

db.o : $(db_depend)
	$(cc) $^ $(flag) -c

back_end.o : $(back_end_depend)
	$(cc) $^ $(flag) -c

iterator.o : $(iterator_depend)
	$(cc) $^ $(flag) -c

hash_table.o : $(hash_table_depend)
	$(cc) $^ $(flag) -c

linked_list.o : $(linked_list_depend)
	$(cc) $^ $(flag) -c

shopping_cart.o : $(shopping_cart_depend)
	$(cc) $^ $(flag) -c

common.o : $(common_depend)
	$(cc) $^ $(flag) -c

utils.o : $(utils_depend)
	$(cc) $^ $(flag) -c

store_specific_data_types.o : $(store_specific_data_types_depend)
	$(cc) $^ $(flag) -c



run: store
	./store


test_all: 
	make -C Back_end/ a

clear:
	make clr $(clr_flags) && make -C Back_end/ clr $(clr_flags)

clr: 
	rm -r hash_table_test linked_test *.out *.o *.gch vgcore.* *.gcov *.gcda *.gcno *.gcda *.info  *.s
	rm -r out

gdb: store
	gdb ./store

valgr: store
	valgrind --leak-check=full  --track-origins=yes  --show-leak-kinds=all ./store


.PHONY: clr run test_all gdb clr
