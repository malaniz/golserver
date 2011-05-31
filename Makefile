all: golp 
CC = gcc 
CFLAGS = -g -pthread -fPIC 
LUAFLAGS = -I/usr/local/include -llua  
golp: job.o golpthread.o  
	$(CC) $(CFLAGS) $(LUAFLAGS) -Wall -shared job.o golpthread.o -o golpthread.so 

golpthread.o: golpthread.c 
	$(CC) $(CFLAGS) -c golpthread.c

job.o: job.c job.h
	$(CC) $(CFLAGS) -c job.c

clean:
	rm -rf *o golp
	rm *.o*
	rm *.e*
