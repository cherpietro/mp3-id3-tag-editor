OBJ= ID3v2.o ID3v2Header.o ID3v2Frames.o SizeReader.o TXTFrameList.o COMMFrameList.o TextString.o PRIVFrameList.o
CFLAGS=-g -Wall -Wextra -pedantic 
CC=gcc

all: compile
	./a.out

compile: clean $(OBJ)
	$(CC) $(CFLAGS) id3.c $(OBJ)

test: clean $(OBJ)
	$(CC) $(CFLAGS) test.c $(OBJ)
	./a.out
	
ID3v2.o: ID3v2.c ID3v2.h ID3v2Header.h
	$(CC) $(CFLAGS) -c ID3v2.c

TXTFrameList.o: TXTFrameList.c TXTFrameList.h
	$(CC) $(CFLAGS) -c TXTFrameList.c

COMMFrameList.o: COMMFrameList.c COMMFrameList.h
	$(CC) $(CFLAGS) -c COMMFrameList.c

PRIVFrameList.o: PRIVFrameList.c PRIVFrameList.h
	$(CC) $(CFLAGS) -c PRIVFrameList.c

ID3v2Frames.o: ID3v2Frames.c ID3v2Frames.h
	$(CC) $(CFLAGS) -c ID3v2Frames.c

ID3v2Header.o: ID3v2Header.c ID3v2Header.h
	$(CC) $(CFLAGS) -c ID3v2Header.c

TextString.o: TextString.c TextString.h
	$(CC) $(CFLAGS) -c TextString.c

SizeReader.o: SizeReader.c SizeReader.h
	$(CC) $(CFLAGS) -c SizeReader.c

clean:
	rm -f a.out *.o cover.jpg
