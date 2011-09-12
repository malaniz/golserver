#include <pthread.h>

int                 MAX_Y;
int                 MAX_X;   
int                 GENERATIONS;
int                 NUM_THREADS;
int                 nboard;
int                 ylong;
int                 xlong;

int**               board;
pthread_t*          threads;
typedef void*       void_t;
pthread_barrier_t   barr;


map_t               topology;

measure_t*          measures;
