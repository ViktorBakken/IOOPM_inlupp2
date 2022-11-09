cc = gcc
flag = -g -Wall -Wextra --coverage -pg
flagExtra = --coverage -pg
CUnit = -lcunit

clr_flags = -k -s -i

hash_table_test_name = a-hash_table_test a-hash_table a-linked_list a-common
hash_table_test.c = hash_table_test.c hash_table.c linked_list.c Generic_func_Data_types/common.c
hash_table_test.o = hash_table_test.o hash_table.o linked_list.o common.o

linked_test_name = a-linked_list_test a-linked_list a-common
linked_test.c = linked_list_test.c linked_list.c iterator.c Generic_func_Data_types/common.c
linked_test.o = linked_list_test.o linked_list.o iterator.o common.o 

iterator_test_name = a-iterator a-iterator_test a-linked_list a-common
iterator_test.c = iterator.c iterator_test.c linked_list.c Generic_func_Data_types/common.c
iterator_test.o = iterator.o iterator_test.o linked_list.o common.o

hash_table_test:$(hash_table_test.o)
	$(cc) $^ $(flag) $(CUnit) -o $@

linked_test: $(linked_test.o)
	$(cc) $^ $(flag) $(CUnit) -o $@

iterator_test: $(iterator_test.o)
	$(cc) $^ $(flag) $(CUnit) -o $@



	
hash_table.o: hash_table.c
	$(cc) hash_table.c $(flag) -c

linked_list.o: linked_list.c
	$(cc) linked_list.c $(flag) -c
	
iterator.o: iterator.c
	$(cc) iterator.c $(flag) -c

common.o: Generic_func_Data_types/common.c
	$(cc) $^ $(flag) -c




hash_table_test.o: $(hash_table_test.c)
	$(cc) $(hash_table_test.c) $(flag) -c


linked_test.o: $(linked_test.c)
	$(cc) $(linked_test.c) $(flag) $(CUnit) -c

iterator_test.o:$(iterator_test.c)
	$(cc) $(iterator_test.c) $(flag) $(CUnit) -c








h: hash_table_test
	./hash_table_test

l: linked_test
	./linked_test

i: iterator_test
	./iterator_test


a: hash_table_test linked_test iterator_test
	echo "hash_table_test"
	valgrind --leak-check=full  --track-origins=yes ./hash_table_test 
	echo ""

	echo "linked_list_test"
	valgrind --leak-check=full  --track-origins=yes ./linked_test
	echo ""

	echo "iterator_test"
	valgrind --leak-check=full  --track-origins=yes ./iterator_test
	echo """

	echo "Done!!"
	# make clr $(clr_flags)


clr: 
	rm -f hash_table_test linked_test *.out *.o *.gch vgcore.* *.gcov *.gcda *.gcno *.gcda *.info  *.s 
	rm -r out
	make -C Generic_func_Data_types/ clr $(clr_flags)



