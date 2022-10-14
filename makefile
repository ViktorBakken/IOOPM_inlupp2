cc = gcc
flag = -g -Wall -Wextra
flagExtra = --coverage -pg
CUnit = -lcunit

clr_flags = -k -s -i

object.c = store.c db.c    Back_end/back_end.c  Back_end/iterator.c Back_end/hash_table.c Back_end/linked_list.c  Back_end/shopping_cart.c   Back_end/Generic_func_Data_types/common.c Back_end/Generic_func_Data_types/utils.c Back_end/Generic_func_Data_types/store_specific_data_types.c 
object.h = store.h db.h    Back_end/back_end.h  Back_end/iterator.h Back_end/hash_table.h Back_end/linked_list.h  Back_end/shopping_cart.h   Back_end/Generic_func_Data_types/common.h Back_end/Generic_func_Data_types/utils.h Back_end/Generic_func_Data_types/store_specific_data_types.h
object.o = store.o db.o back_end.o iterator.o hash_table.o linked_list.o shopping_cart.o common.o utils.o store_specific_data_types.o
object_test = store_test.c

store: $(object.o)
	$(cc) $^ $(flag) -o store

%.o : $(object.c) $(object.h)
	$(cc) $^ $(flag) -c

# %_test.o : $(object.c)
# 	$(cc) $^ $(flag) object_test.c $(CUnit) -c

run: store
	./store


test_all: 
	make -C Back_end/ a

clear:
	make clr $(clr_flags) && make -C Back_end/ clr $(clr_flags)

clr: 
	rm -r hash_table_test linked_test *.out *.o *.gch vgcore.* *.gcov *.gcda *.gcno *.gcda *.info  *.s
	rm -r out

gdb: 
	gdb ./store

.PHONY: clr run test_all
