CC = g++
COMPILE = -c
DEBUG = -g
CFLAGS = -ldl

thread.o: thread.cc
	$(CC) $(COMPILE) $(DEBUG) thread.cc

test0: test0a test0b
	printf "\n--------- OURS ---------\n" && ./test0a && printf "\n-------- THEIRS ---------\n" && ./test0b

test0a: test0.cc thread.o
	$(CC) $(DEBUG) -o test0a thread.o test0.cc libinterrupt.a $(CFLAGS)

test0b: test0.cc thread-correct.o
	$(CC) $(DEBUG) -o test0b thread-correct.o test0.cc libinterrupt.a $(CFLAGS)

test1: test1a test1b
	printf "\n--------- OURS ---------\n" && ./test1a && printf "\n-------- THEIRS ---------\n" && ./test1b

test1a: test1.cc thread.o
	$(CC) $(DEBUG) -o test1a thread.o test1.cc libinterrupt.a $(CFLAGS)

test1b: test1.cc thread-correct.o
	$(CC) $(DEBUG) -o test1b thread-correct.o test1.cc libinterrupt.a $(CFLAGS)

test2: test2a test2b
	printf "\n--------- OURS ---------\n" && ./test2a && printf "\n-------- THEIRS ---------\n" && ./test2b

test2a: test2.cc thread.o
	$(CC) $(DEBUG) -o test2a thread.o test2.cc libinterrupt.a $(CFLAGS)

test2b: test2.cc thread-correct.o
	$(CC) $(DEBUG) -o test2b thread-correct.o test2.cc libinterrupt.a $(CFLAGS)

test3: test3a test3b
	printf "\n--------- OURS ---------\n" && ./test3a && printf "\n-------- THEIRS ---------\n" && ./test3b

test3a: test3.cc thread.o
	$(CC) $(DEBUG) -o test3a thread.o test3.cc libinterrupt.a $(CFLAGS)

test3b: test3.cc thread-correct.o
	$(CC) $(DEBUG) -o test3b thread-correct.o test3.cc libinterrupt.a $(CFLAGS)

test4: test4a test4b
	printf "\n--------- OURS ---------\n" && ./test4a && printf "\n-------- THEIRS ---------\n" && ./test4b

test4a: test4.cc thread.o
	$(CC) $(DEBUG) -o test4a thread.o test4.cc libinterrupt.a $(CFLAGS)

test4b: test4.cc thread-correct.o
	$(CC) $(DEBUG) -o test4b thread-correct.o test4.cc libinterrupt.a $(CFLAGS)

test5: test5a test5b
	printf "\n--------- OURS ---------\n" && ./test5a && printf "\n-------- THEIRS ---------\n" && ./test5b

test5a: test5.cc thread.o
	$(CC) $(DEBUG) -o test5a thread.o test5.cc libinterrupt.a $(CFLAGS)

test5b: test5.cc thread-correct.o
	$(CC) $(DEBUG) -o test5b thread-correct.o test5.cc libinterrupt.a $(CFLAGS)


test10: test10a test10b
	printf "\n--------- OURS ---------\n" && ./test10a && printf "\n-------- THEIRS ---------\n" && ./test10b

test10a: test10.cc thread.o
	$(CC) $(DEBUG) -o test10a thread.o test10.cc libinterrupt.a $(CFLAGS)

test10b: test10.cc thread-correct.o
	$(CC) $(DEBUG) -o test10b thread-correct.o test10.cc libinterrupt.a $(CFLAGS)

test20: test20a test20b
	printf "\n--------- OURS ---------\n" && ./test20a && printf "\n-------- THEIRS ---------\n" && ./test20b

test20a: test20.cc thread.o
	$(CC) $(DEBUG) -o test20a thread.o test20.cc libinterrupt.a $(CFLAGS)

test20b: test20.cc thread-correct.o
	$(CC) $(DEBUG) -o test20b thread-correct.o test20.cc libinterrupt.a $(CFLAGS)

test21: test21a test21b
	printf "\n--------- OURS ---------\n" && ./test21a && printf "\n-------- THEIRS ---------\n" && ./test21b

test21a: test21.cc thread.o
	$(CC) $(DEBUG) -o test21a thread.o test21.cc libinterrupt.a $(CFLAGS)

test21b: test21.cc thread-correct.o
	$(CC) $(DEBUG) -o test21b thread-correct.o test21.cc libinterrupt.a $(CFLAGS)

test22: test22a test22b
	printf "\n--------- OURS ---------\n" && ./test22a && printf "\n-------- THEIRS ---------\n" && ./test22b

test22a: test22.cc thread.o
	$(CC) $(DEBUG) -o test22a thread.o test22.cc libinterrupt.a $(CFLAGS)

test22b: test22.cc thread-correct.o
	$(CC) $(DEBUG) -o test22b thread-correct.o test22.cc libinterrupt.a $(CFLAGS)

test23: test23a test23b
	printf "\n--------- OURS ---------\n" && ./test23a && printf "\n-------- THEIRS ---------\n" && ./test23b

test23a: test23.cc thread.o
	$(CC) $(DEBUG) -o test23a thread.o test23.cc libinterrupt.a $(CFLAGS)

test23b: test23.cc thread-correct.o
	$(CC) $(DEBUG) -o test23b thread-correct.o test23.cc libinterrupt.a $(CFLAGS)

test25: test25a test25b
	printf "\n--------- OURS ---------\n" && ./test25a && printf "\n-------- THEIRS ---------\n" && ./test25b

test25a: test25.cc thread.o
	$(CC) $(DEBUG) -o test25a thread.o test25.cc libinterrupt.a $(CFLAGS)

test25b: test25.cc thread-correct.o
	$(CC) $(DEBUG) -o test25b thread-correct.o test25.cc libinterrupt.a $(CFLAGS)
clean:
	rm -f test{0..30}a test{0..30}b thread.o
