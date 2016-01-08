# You can compile with either gcc or g++
# CC = g++
CC = gcc
CFLAGS = -I. -Wall -lm
# disable the -DNDEBUG flag for the printing the freelist
OPTFLAG = -O2
DEBUGFLAG = -g

all: test_dmalloc test_free quick_test test_coalesce

dmm.o: dmm.c
	$(CC) $(DEBUGFLAG) $(CFLAGS) -c dmm.c 

test_dmalloc: test_dmalloc.c dmm.o
	$(CC) $(DEBUGFLAG) $(CFLAGS) -o $@ test_dmalloc.c dmm.o

test_free: test_free.c dmm.o
	$(CC) $(DEBUGFLAG) $(CFLAGS) -o $@ test_free.c dmm.o
	
test_coalesce: test_coalesce.c dmm.o
	$(CC) $(DEBUGFLAG) $(CFLAGS) -o $@ test_coalesce.c dmm.o
	
quick_test: quick_test.c dmm.o
	$(CC) $(DEBUGFLAG) $(CFLAGS) -o $@ quick_test.c dmm.o

tests: test_dmalloc test_free test_coalesce quick_test 
	./test_dmalloc && ./test_free && ./quick_test && ./test_coalesce
	
clean:
	rm -rf *.o *.dSYM test_dmalloc test_free test coalesce quick_test
