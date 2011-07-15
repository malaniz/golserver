all: golpthread 
CC = gcc 
CFLAGS = -g -Wall -pthread -fPIC -lrt 
LUAFLAGS = -I/usr/local/include -llua  
HWLOCCFLAGS = -I/usr/local/include -I/usr/include/libxml2
HWLOCLIB = -L/usr/local/lib -lhwloc



golpthread: job.o golpthread.o mapping.o measure.o
	$(CC) $(CFLAGS) $(LUAFLAGS) $(HWLOCLIB) -shared mapping.o job.o golpthread.o measure.o -o golpthread.so 

mapping.o: mapping.h mapping.c
	$(CC) $(CFLAGS) $(HWLOCCFLAGS) $(HWLOCLIB) -c mapping.c

golpthread.o: golpthread.c 
	$(CC) $(CFLAGS) -c golpthread.c

job.o: job.c job.h
	$(CC) $(CFLAGS) -c job.c

measure.o: measure.h measure.c
	$(CC) $(CFLAGS) -c measure.c

clean:
	rm -rf *o golpthread.so 
