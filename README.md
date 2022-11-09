Earlier assignment libraries:
Name:               From:
Common.c/.h         Viktor B./Egon H.
Hash_table.c/.h     
Linked_list.c/.h    
iterator.c/ .h

items_db.c/.h       Loran D./Simon P
utils.c/.h          Loran D./Simon P


Build and run:
    store: make run
    tests: make test_all

    run program with valgrind: make valgr
    run program with gdb: make gdb
    lcov: make clear & make lcov_all   (to use lcov, you need to first of all go into both makefile (and Back_end/makefile) and uncomment the flags --coverage -pg in the flag variable)


(Stats taken from lcov)
Files             Line Coverage (%)      Function Coverage(%)
item_db.c:            83.3%                    94.1%
item_db_back_end.c:   90.5%                    100%
shopping_cart.c:      75.7%                    84.2%


How to use program:
    To start the program you enter "make run"

    Files needed to run program:
        store.c /.h

        items_db.c /.h
        items_db_back_end.c /.h
        shopping_cart.c /.h

        hash_table.c /.h
        linked_list.c /.h
        iterator.c /.h

        common.c /.h
        utils.c /.h
        store_specific_data_types.c /.h