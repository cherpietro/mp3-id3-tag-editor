OBJ= ID3v2.o ID3v2Header.o ID3v2Frames.o SizeReader.o  TextString.o FileFrameManager.o PrintFrameManager.o ListFramePtr.o DeleteFrameManager.o AddFrameManager.o StoreFrameManager.o SizeFrameManager.o GetFrameManager.o
CFLAGS=-g -Wall -Wextra -pedantic 
CC=gcc
DIRS=files savedFiles

all:create_dirs compile clear
	./a.out

compile: create_dirs clean $(OBJ)
	$(CC) $(CFLAGS) main.c $(OBJ)

create_dirs:
	mkdir -p $(DIRS)

test: clean $(OBJ)
	$(CC) $(CFLAGS) test.c $(OBJ)
	./a.out
	
ID3v2.o: ID3v2.c ID3v2.h ID3v2Header.h
	$(CC) $(CFLAGS) -c ID3v2.c

ListFramePtr.o: ListFramePtr.c ListFramePtr.h
	$(CC) $(CFLAGS) -c ListFramePtr.c
	
GetFrameManager.o: FrameManager/GetFrameManager.c FrameManager/GetFrameManager.h
	$(CC) $(CFLAGS) -c FrameManager/GetFrameManager.c

DeleteFrameManager.o: FrameManager/DeleteFrameManager.c FrameManager/DeleteFrameManager.h
	$(CC) $(CFLAGS) -c FrameManager/DeleteFrameManager.c

StoreFrameManager.o: FrameManager/StoreFrameManager.c FrameManager/StoreFrameManager.h
	$(CC) $(CFLAGS) -c FrameManager/StoreFrameManager.c

SizeFrameManager.o: FrameManager/SizeFrameManager.c FrameManager/SizeFrameManager.h
	$(CC) $(CFLAGS) -c FrameManager/SizeFrameManager.c

AddFrameManager.o: FrameManager/AddFrameManager.c FrameManager/AddFrameManager.h
	$(CC) $(CFLAGS) -c FrameManager/AddFrameManager.c
	
FileFrameManager.o: FrameManager/FileFrameManager.c FrameManager/FileFrameManager.h
	$(CC) $(CFLAGS) -c FrameManager/FileFrameManager.c
	
PrintFrameManager.o: FrameManager/PrintFrameManager.c FrameManager/PrintFrameManager.h
	$(CC) $(CFLAGS) -c FrameManager/PrintFrameManager.c

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

clear:
	rm -f  *.o
