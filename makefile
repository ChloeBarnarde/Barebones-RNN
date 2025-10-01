

Test:
	$(CC) src/test/matrixMathTest.c src/matrixMath.h src/matrixMath.c -lm -o test.out
	./test.out

