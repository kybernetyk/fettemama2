CC=g++
LIBS=
CFLAGS=-std=c++0x

EXECNAME=blog

all:	blog

test: clean all run

blog: ccfiles
	$(CC) $(LIBS) *.o -o $(EXECNAME)

ccfiles:
	$(CC) $(CFLAGS) *.cc

clean:
	rm -rf *.o $(EXECNAME)

run:
	./blog
