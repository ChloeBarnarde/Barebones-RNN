demo: matrixMath.o main.o
	$(CC) main.o matrixMath.o -lm -o demo.out

main.o: main.c
	$(CC) -c src/main.c -o main.o

main.c:

# testing 
test: test.o matrixMath.o 
	$(CC) test.o matrixMath.o -lm -o test.out
	./test.out

test.o:
	$(CC) -c src/test/matrixMathTest.c -o test.o

matrixMath.o:
	$(CC) -c src/matrixMath.c src/matrixMath.h
