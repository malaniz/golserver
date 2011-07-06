all: golp 
CC = gcc 
CFLAGS = -g -pthread -fPIC 
LUAFLAGS = -I/usr/local/include -llua  
HWLOCCFLAGS = -I/usr/local/include -I/usr/include/libxml2
HWLOCLIB = -L/usr/local/lib -lhwloc



golp: job.o golpthread.o mapping.o 
	$(CC) $(CFLAGS) $(LUAFLAGS) $(HWLOCLIB) -shared mapping.o job.o golpthread.o -o golpthread.so 

golpthread.o: golpthread.c 
	$(CC) $(CFLAGS) -c golpthread.c

job.o: job.c job.h
	$(CC) $(CFLAGS) -c job.c

mapping.o: mapping.h mapping.c
	$(CC) $(CFLAGS) $(HWLOCCFLAGS) $(HWLOCLIB) -c mapping.c

clean:
	rm -rf *o golpthread.so 
