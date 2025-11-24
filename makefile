demo: matrixMath.o main.o rnn.o
	$(CC) main.o matrixMath.o rnn.o -lm -o demo.out

main.o: main.c
	$(CC) -c src/main.c -o main.o

main.c:

rnn.o: rnn.c
	$(CC) -c src/rnn.c src/rnn.h

rnn.c:

# testing 
test: test.o matrixMath.o 
	$(CC) test.o matrixMath.o -lm -o test.out
	./test.out

test.o:
	$(CC) -c src/test/matrixMathTest.c -o test.o

matrixMath.o: matrixMath.c
	$(CC) -c src/matrixMath.c src/matrixMath.h

matrixMath.c:

