
all: clean compile
	./a.out
compile:
	gcc -g -c -W ID3v2.c  && gcc -g -W id3.c ID3v2.o

clean:
	rm -f a.out ID3v2.o cover.jpg