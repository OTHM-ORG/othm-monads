all:
	gcc -o test test.c othm_noMonads.c -lothm_symbols -lothm_base -lothm_hashmap
