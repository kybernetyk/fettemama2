CC=g++
LIBS=-lcurl -pthread -levent -L/usr/lib -lmysqlclient -lpthread -lz -lm -lrt -lssl -lcrypto -ldl
CFLAGS=-std=c++0x -I/usr/include/mysql -fPIC -pipe -fstack-protector --param=ssp-buffer-size=4 -D_FORTIFY_SOURCE=2 -fno-strict-aliasing -DBIG_JOINS=1 -fomit-frame-pointer  -g
EXECNAME=blog

all:	blog

test: clean all run

blog:
	$(CC) $(CFLAGS) *.cc $(LIBS) -o $(EXECNAME)

clean:
	rm -rf *.o $(EXECNAME) core

run: blog
	./blog
