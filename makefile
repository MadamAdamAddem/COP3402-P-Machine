all:
	gcc -O2 -Wall -std=c11 -o vm vm.c && clear && ./vm in 