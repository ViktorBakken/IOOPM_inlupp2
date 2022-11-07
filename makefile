cc = gcc
flag = -g -Wall -Wextra --coverage -pg
# flagExtra = 
CUnit = -lcunit

clr_flags = -k -s -i

# Which directory is the file stored at
store_plac = store.c
db_test_plac = db_test.c
shopping_cart_test_plac = Back_end/shopping_cart_test.c

db_plac = db.c
db_back_end_plac = Back_end/db_back_end.c 

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
store_depend = $(store_plac) $(db_plac) $(db_back_end_plac)  $(iterator_plac) $(hash_table_plac) $(linked_list_plac)  $(shopping_cart_plac)   $(common_plac) $(utils_plac) $(store_specific_data_types_plac)

shopping_cart_depend = $(shopping_cart_plac) $(db_plac) $(store_specific_data_types_plac) $(datatypes_plac)
shopping_cart_test_depend = $(shopping_cart_test_plac) $(shopping_cart_depend) $(db_back_end_plac)

db_depend = $(db_plac) $(db_back_end_plac) $(utils_plac) $(store_specific_data_types_plac) $(datatypes_plac)
db_back_end_depend = $(db_back_end_plac) $(db_depend)
db_test_depend = $(db_depend) $(db_test_plac)


iterator_depend = $(iterator_plac) $(common_plac)
hash_table_depend =$(hash_table_plac) $(linked_list_plac) $(common_plac)
linked_list_depend = $(linked_list_plac) $(iterator_plac) $(common_plac)

utils_depend = $(utils_plac)
common_depend = $(common_plac)
store_specific_data_types_depend = $(store_specific_data_types_plac)


datatypes.o = iterator.o hash_table.o linked_list.o
#Runable programs
store.o = store.o db.o db_back_end.o iterator.o hash_table.o linked_list.o shopping_cart.o common.o utils.o store_specific_data_types.o

db_test.o = db_test.o db.o db_back_end.o utils.o store_specific_data_types.o utils.o common.o $(datatypes.o)
shopping_cart_test.o = shopping_cart_test.o shopping_cart.o store_specific_data_types.o db_back_end.o db.o $(datatypes.o) utils.o common.o


%.o: %.c 
	$(cc) $^ $(flag) -c

%_test.o : %_test.c
	$(cc) $^ $(flag) $(CUnit) -c


store: $(store.o)
	$(cc) $^ $(flag) -o store


tests = cart_test db_test

db_test: $(db_test.o)
	$(cc) $^ $(flag) $(CUnit) -o $@

cart_test: $(shopping_cart_test.o) #$(store.o)
	$(cc) $^ $(flag) $(CUnit) -o $@




# store.o : $(store_depend)
# 	$(cc) $^ $(flag) -c

# db.o : $(db_depend)
# 	$(cc) $^ $(flag) -c

db_test.o: $(db_test_depend)
	$(cc) $^ $(flag) $(CUnit) -c

db_back_end.o : $(db_back_end_depend)
	$(cc) $^ $(flag) -c

# iterator.o : $(iterator_depend)
# 	$(cc) $^ $(flag) -c

# hash_table.o : $(hash_table_depend)
# 	$(cc) $^ $(flag) -c

# linked_list.o : $(linked_list_depend)
# 	$(cc) $^ $(flag) -c

shopping_cart.o : $(shopping_cart_depend)
	$(cc) $^ $(flag) -c

shopping_cart_test.o : $(shopping_cart_test_depend)
	$(cc) $^ $(flag) $(CUnit) -c

common.o : $(common_depend)
	$(cc) $^ $(flag) -c

utils.o : $(utils_depend)
	$(cc) $^ $(flag) -c

# store_specific_data_types.o : $(store_specific_data_types_depend)
# 	$(cc) $^ $(flag) -c


all = $(store_depend)  $(shopping_cart_test_plac)
db_special_comp: $(db_test_plac)
	make clr $(clr_flags)
	$(cc) $^ $(flag) $(flagExtra) $(CUnit)

gprof: special_comp
	./a.out
	gprof -b a.out gmon.out > analysis.txt && more analysis.txt

callgrind: special_comp
	valgrind --tool=callgrind --callgrind-out-file=callgrind_test.out ./a.out 	
	kcachegrind callgrind_test.out

lcov: special_comp
	./cart_test
	./db_test
	lcov --capture --directory . --output-file coverage.info && genhtml coverage.info --output-directory out 

gcov: special_comp
	./a.out && gcov -abcfu a-$($(progr)_name)



run: store
	./store


test_all: $(tests)
	./cart_test
	./db_test
	# make -C Back_end/ a

clear:
	make clr $(clr_flags) && make -C Back_end/ clr $(clr_flags)

clr: 
	rm -r hash_table_test linked_test cart_test store *.out *.o *.gch vgcore.* *.gcov *.gcda *.gcno *.gcda *.info  *.s
	rm -r out

gdb: store
	gdb ./store

valgr: store
	valgrind --leak-check=full  --track-origins=yes  --show-leak-kinds=all ./store


.PHONY: clr run test_all gdb valgr clr
