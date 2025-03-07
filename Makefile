OBJ= ID3v2.o ID3v2Header.o ID3v2Frames.o SizeReader.o
CFLAGS=-g -Wall -Wextra -pedantic 
all: compile
	./a.out

compile: clean $(OBJ)
	gcc $(CFLAGS) id3.c $(OBJ)

oldCompile:
	gcc -g -Wall -Wextra -pedantic -c ID3v2.c  && gcc  -g -Wall -Wextra -pedantic id3.c ID3v2.o


ID3v2.o: ID3v2.c ID3v2.h ID3v2Header.h
	gcc $(CFLAGS) -c ID3v2.c

ID3v2Frames.o: ID3v2Frames.c ID3v2Frames.h
	gcc $(CFLAGS) -c ID3v2Frames.c

ID3v2Header.o: ID3v2Header.c ID3v2Header.h
	gcc $(CFLAGS) -c ID3v2Header.c

SizeReader.o.o: SizeReader.o. SizeReader.o 
	gcc $(CFLAGS) -c SizeReader.o 

clean:
	rm -f a.out *.o cover.jpg