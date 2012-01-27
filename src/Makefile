CC=g++
LIBS=-lcurl -pthread -levent
CFLAGS=-std=c++0x

EXECNAME=blog

all:	blog

test: clean all run

blog:
	$(CC) $(CFLAGS) *.cc $(LIBS) -o $(EXECNAME)

clean:
	rm -rf *.o $(EXECNAME)

run: blog
	./blog
