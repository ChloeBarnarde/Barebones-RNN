
# testing 
test: test.o matrixMath.o 
	$(CC) test.o matrixMath.o -lm -o test.out
	./test.out

test.o:
	$(CC) -c src/test/matrixMathTest.c -o test.o

matrixMath.o:
	$(CC) -c src/matrixMath.c src/matrixMath.h
