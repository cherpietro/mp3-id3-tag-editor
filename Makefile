OBJ= ID3v2.o ID3v2Header.o ID3v2Frames.o SizeReader.o TextFrameStack.o
CFLAGS=-g -Wall -Wextra -pedantic 
CC=gcc

all: compile
	./a.out

compile: clean $(OBJ)
	$(CC) $(CFLAGS) id3.c $(OBJ)

ID3v2.o: ID3v2.c ID3v2.h ID3v2Header.h
	$(CC) $(CFLAGS) -c ID3v2.c

TextFrameStack.o: TextFrameStack.c TextFrameStack.h
	$(CC) $(CFLAGS) -c TextFrameStack.c

ID3v2Frames.o: ID3v2Frames.c ID3v2Frames.h
	$(CC) $(CFLAGS) -c ID3v2Frames.c

ID3v2Header.o: ID3v2Header.c ID3v2Header.h
	$(CC) $(CFLAGS) -c ID3v2Header.c

SizeReader.o: SizeReader.c SizeReader.h
	$(CC) $(CFLAGS) -c SizeReader.c

clean:
	rm -f a.out *.o cover.jpg
