mycomp: mycomp.o complex.o
	gcc -g -ansi -Wall -pedantic mycomp.o complex.o -o mycomp -lm

mycomp.o: mycomp.c complex.h
	gcc -c -ansi -Wall -pedantic mycomp.c -o mycomp.o

complex.o: complex.c complex.h
	gcc -c -ansi -Wall -pedantic complex.c -o complex.o

clean:
	rm -f *.o mycomp