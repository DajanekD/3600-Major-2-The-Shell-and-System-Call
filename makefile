major2: major2.o shell.o
	gcc -Wall -o major2 major2.o shell.o

major2.o: major2.h
	gcc -c major2.c

shell.o: major2.h
	gcc -c shell.c

rebuild:
	make clean
	make
	clear
	make run

clean:
	rm major2.o
	rm shell.o
	rm major2

run:
	clear
	./major2
