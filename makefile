calc: calc.o
	gcc -g -std=c99 -Wall -pedantic calc.o -o calc

calc.o: calc.c
	gcc -O2 -std=c99 -c -Wall -pedantic calc.c -o calc.o

mycomp: mycomp.o complex.o
	gcc -g -ansi -Wall -pedantic mycomp.o complex.o -o mycomp -lm

mycomp.o: mycomp.c complex.h
	gcc -c -ansi -Wall -pedantic mycomp.c -o mycomp.o

complex.o: complex.c complex.h
	gcc -c -ansi -Wall -pedantic complex.c -o complex.o

clean:
	rm -f *.o mycomp
	rm -f *.o calc
