CC=gcc
CFLAGS= -Wall -Wextra -Werror -std=c99
NAME= c_collector

all:
	$(CC) $(CFLAGS) $(NAME)_lib.c -o $(NAME)
	

clear:
	rm test

	
test:
	$(CC) $(CFLAGS) tests.c -c
	$(CC) $(CFLAGS) $(NAME)_lib.c -c
	$(CC) tests.o $(NAME)_lib.o -o test
	rm *.o
