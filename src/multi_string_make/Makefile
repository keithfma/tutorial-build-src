write: main.o WriteMyString.o
	gcc main.o WriteMyString.o -o write

main.o: main.c header.h
	gcc -c main.c

WriteMyString.o: WriteMyString.c
	gcc -c WriteMyString.c

clean: 
	rm write *.o
