CC = g++
CFLAGS = -ldl

all: thread.o deli.cc libinterrupt.a
	$(CC) -o deli thread.o deli.cc libinterrupt.a $(CFLAGS)

test: sw.in0 sw.in1 sw.in2 sw.in3 sw.in4 deli
	./deli 3 sw.in0 sw.in1 sw.in2 sw.in3 sw.in4 sw.in5

