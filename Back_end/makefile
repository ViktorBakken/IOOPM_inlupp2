progr = hash_table_test

cc = gcc
flag = -g -Wall -Wextra
flagExtra = --coverage -pg
CUnit = -lcunit

clr_flags = -k -s -i

hash_table_test_name = a-hash_table_test a-hash_table a-linked_list a-common
hash_table_test.c = hash_table_test.c hash_table.c linked_list.c Generic_func_Data_types/common.c
hash_table_test.o = hash_table_test.o hash_table.o linked_list.o Generic_func_Data_types/common.o

linked_test_name = a-linked_list_test a-linked_list a-common
linked_test.c = linked_list_test.c linked_list.c Generic_func_Data_types/common.c
linked_test.o = linked_list_test.o linked_list.o Generic_func_Data_types/common.o 






# %: %.o
# 	$(cc) $^ $(flag) $(CUnit) -o $@

# %.o: $(wildcard %.c)
# 	$(cc) $^ $(flag) -c

# %_test.o: $(wildcard %_test.c)
#  	$(cc) $^ $(flag) $(CUnit) -c


# hash_table_test:$(hash_table_test.o)
# 	$(cc) $(flag) $(CUnit) -o $@

# linked_test: $(linked_test.o)
# 	$(cc) $(linked_test.o) $(flag) $(CUnit) -o $@



	
hash_table.o: hash_table.c
	$(cc) hash_table.c $(flag) -c

linked_list.o: linked_list.c
	$(cc) linked_list.c $(flag) -c


common.o: common.c
	$(cc) common.c $(flag) -c




hash_table_test.o: $(hash_table_test.c)
	$(cc) $(hash_table_test.c) $(flag) -c


linked_test.o: $(linked_test.c)
	$(cc) $(linked_test.c) $(flag) $(CUnit) -c




$(progr)_special_comp: $($(progr).c)
	make clr $(clr_flags) && $(cc) $($(progr).c) $(flag) $(flagExtra) $(CUnit)

gprof: $(progr)_special_comp
	./a.out && gprof -b a.out gmon.out > analysis.txt && more analysis.txt

callgrind: $(progr)_special_comp
	valgrind --tool=callgrind --callgrind-out-file=callgrind_test.out ./a.out 	
	kcachegrind callgrind_test.out

lcov: $(progr)_special_comp
	./a.out && lcov --capture --directory . --output-file coverage.info && genhtml coverage.info --output-directory out 

gcov: $(progr)_special_comp
	./a.out && gcov -abcfu a-$($(progr)_name)









h: hash_table_test
	./hash_table_test

l: linked_test
	./linked_test


a: hash_table_test linked_test 
	valgrind --leak-check=full  --track-origins=yes ./hash_table_test  && valgrind --leak-check=full  --track-origins=yes ./linked_test && make clr $(clr_flags)


clr: 
	rm -f hash_table_test linked_test *.out *.o *.gch vgcore.* *.gcov *.gcda *.gcno *.gcda *.info  *.s 
	rm -r out
	make -C Generic_func_Data_types/ clr $(clr_flags)

valgr: $(progr)
	valgrind --leak-check=full  --track-origins=yes  --show-leak-kinds=all ./$(progr)

gdb: $(progr)  
	gdb ./$(progr)  



