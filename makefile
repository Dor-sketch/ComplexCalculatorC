mycomp: mycomp.o complex.o gui.o
	gcc -g -ansi -Wall -pedantic mycomp.o complex.o gui.o -o mycomp -lm `pkg-config --cflags --libs gtk+-3.0`

mycomp.o: mycomp.c complex.h
	gcc -c -ansi -Wall -pedantic mycomp.c -o mycomp.o

complex.o: complex.c complex.h
	gcc -c -ansi -Wall -pedantic complex.c -o complex.o

gui.o: gui.c
	gcc -c -Wall -pedantic gui.c -o gui.o `pkg-config --cflags gtk+-3.0`

clean:
	rm -f *.o mycomp