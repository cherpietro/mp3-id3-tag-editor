
all: clean compile
	./a.out
compile:
	gcc -g -Wall -Wextra -pedantic -c ID3v2.c  && gcc  -g -Wall -Wextra -pedantic id3.c ID3v2.o

clean:
	rm -f a.out ID3v2.o cover.jpg