cc = gcc
flag = -g -Wall -Wextra #--coverage -pg
flagExtra = --coverage -pg
CUnit = -lcunit

make_flags = -k -s -i

# Which directory is the file stored at
store_plac = store.c
items_db_test_plac = items_db_test.c
cart_test_plac = Back_end/shopping_cart_test.c

items_db_plac = items_db.c
items_db_back_end_plac = Back_end/items_db_back_end.c 

shopping_cart_plac = Back_end/shopping_cart.c 
shopping_cart_backend_plac =  Back_end/shopping_cart_backend.c

datatypes_plac = $(iterator_plac) $(hash_table_plac) $(linked_list_plac)
iterator_plac = Back_end/iterator.c 
hash_table_plac = Back_end/hash_table.c 
linked_list_plac = Back_end/linked_list.c

common_plac = Back_end/Generic_func_Data_types/common.c
utils_plac = Back_end/Generic_func_Data_types/utils.c
store_specific_data_types_plac = Back_end/Generic_func_Data_types/store_specific_data_types.c 


#The dependecies of each file
store_depend = $(store_plac) $(items_db_plac) $(items_db_back_end_plac)  $(iterator_plac) $(hash_table_plac) $(linked_list_plac)  $(shopping_cart_plac)   $(common_plac) $(utils_plac) $(store_specific_data_types_plac)

shopping_cart_depend = $(shopping_cart_plac) $(items_db_plac) $(store_specific_data_types_plac) $(common_plac)  $(utils_plac) $(datatypes_plac)
cart_test_depend = $(cart_test_plac) $(shopping_cart_depend) $(items_db_back_end_plac)

items_db_depend = $(items_db_plac) $(items_db_back_end_plac) $(utils_plac) $(store_specific_data_types_plac)$(common_plac) $(datatypes_plac)
items_db_back_end_depend = $(items_db_back_end_plac) $(items_db_depend)
items_db_test_depend = $(items_db_depend) $(items_db_test_plac)


iterator_depend = $(iterator_plac) $(common_plac)
hash_table_depend =$(hash_table_plac) $(linked_list_plac) $(common_plac)
linked_list_depend = $(linked_list_plac) $(iterator_plac) $(common_plac)

utils_depend = $(utils_plac)
common_depend = $(common_plac)
store_specific_data_types_depend = $(store_specific_data_types_plac)


datatypes.o = iterator.o hash_table.o linked_list.o
#Runable programs
store.o = store.o items_db.o items_db_back_end.o iterator.o hash_table.o linked_list.o shopping_cart.o common.o utils.o store_specific_data_types.o

items_db_test.o = items_db_test.o items_db.o items_db_back_end.o utils.o store_specific_data_types.o utils.o common.o $(datatypes.o)
cart_test.o = shopping_cart_test.o shopping_cart.o store_specific_data_types.o items_db_back_end.o items_db.o $(datatypes.o) utils.o common.o


%.o: %.c 
	$(cc) $^ $(flag) -c

%_test.o : %_test.c
	$(cc) $^ $(flag) $(CUnit) -c


store: $(store.o)
	$(cc) $^ $(flag) -o store


tests = cart_test items_db_test

items_db_test: $(items_db_test.o)
	$(cc) $^ $(flag) $(CUnit) -o $@

cart_test: $(cart_test.o)
	$(cc) $^ $(flag) $(CUnit) -o $@




# store.o : $(store_depend)
# 	$(cc) $^ $(flag) -c

# items_db.o : $(items_db_depend)
# 	$(cc) $^ $(flag) -c

items_db_test.o: $(items_db_test_depend)
	$(cc) $^ $(flag) $(CUnit) -c

items_db_back_end.o : $(items_db_back_end_depend)
	$(cc) $^ $(flag) -c

# iterator.o : $(iterator_depend)
# 	$(cc) $^ $(flag) -c

# hash_table.o : $(hash_table_depend)
# 	$(cc) $^ $(flag) -c

# linked_list.o : $(linked_list_depend)
# 	$(cc) $^ $(flag) -c

shopping_cart.o : $(shopping_cart_depend)
	$(cc) $^ $(flag) -c

shopping_cart_test.o : $(cart_test_depend)
	$(cc) $^ $(flag) $(CUnit) -c

common.o : $(common_depend)
	$(cc) $^ $(flag) -c

utils.o : $(utils_depend)
	$(cc) $^ $(flag) -c

# store_specific_data_types.o : $(store_specific_data_types_depend)
# 	$(cc) $^ $(flag) -c

progr = cart_test

$(progr)_special_comp: $($(progr)_depend)
	make clr $(make_flags) 
	$(cc) $^ $(flag) $(flagExtra) $(CUnit)

gprof: $(progr)_special_comp
	./a.out
	gprof -b a.out gmon.out > analysis.txt
	more analysis.txt

callgrind: $(progr)_special_comp
	valgrind --tool=callgrind --callgrind-out-file=callgrind_test.out ./a.out 	
	kcachegrind callgrind_test.out

lcov: $(progr)_special_comp
	./a.out
	lcov --capture --directory . --output-file coverage.info && genhtml coverage.info --output-directory out 


lcov_all: $(tests)
	make jummy_tests  $(make_flags)
	lcov --capture --directory . --output-file coverage.info && genhtml coverage.info --output-directory out 

# gcov: $(progr)_special_comp
# 	./a.out && gcov -abcfu a-$($(progr)_name)



run: store
	./store


test_all:
	make jummy_tests $(make_flags)
	
jummy_tests:$(tests)
	echo "cart_test"
	valgrind --leak-check=full  --track-origins=yes ./cart_test
	echo ""

	echo "items_db_test"
	valgrind --leak-check=full  --track-origins=yes ./items_db_test
	echo ""

	make -C Back_end/ a

clear:
	make clr $(make_flags) && make -C Back_end/ clr $(make_flags)

clr: 
	rm -r hash_table_test linked_test cart_test store *.out *.o *.gch vgcore.* *.gcov *.gcda *.gcno *.gcda *.info  *.s
	rm -r out

gdb: store
	gdb ./store

valgr: store
	valgrind --leak-check=full  --track-origins=yes  --show-leak-kinds=all ./store


.PHONY: clr run test_all gdb valgr clr
