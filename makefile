cc = gcc
flag = -g -Wall -Wextra
flagExtra = --coverage -pg
CUnit = -lcunit

clr_flags = -k -s -i

object.c = store.c Back_end/back_end.c Back_end/Generic_func_Data_types/common.c
object.o = store.o back_end.o common.o
object_test = store_test.c

store: $(object.o)
	$(cc) $^ $(flag) -o store

%.o : $(object.c)
	$(cc) $^ $(flag) -c

%_test.o : $(object.c)
	$(cc) $^ $(flag) object_test.c $(CUnit) -c

run: store
	./store


test_all: 
	make -C Back_end/ a

clear:
	make clr $(clr_flags) && make -C Back_end/ clr $(clr_flags)

clr: 
	rm -f hash_table_test linked_test *.out *.o *.gch vgcore.* *.gcov *.gcda *.gcno *.gcda *.info  *.s !makefile.o
	rm -r out


