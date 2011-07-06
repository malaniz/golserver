#include <pthread.h>
#define MAX_Y 40 
#define MAX_X 80 

// nro of threads
#define NUM_THREADS 4

#define nboard 2
#define ylong 20
#define xlong 40
/*
int NUM_THREADS;
int nboard;
int ylong;
int xlong;
*/
int                 board[MAX_Y][MAX_X];
pthread_t*          threads;
typedef void*       void_t;
pthread_barrier_t   barr;


