#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include "mapping.h"


#define MAX_Y 40 
#define MAX_X 80 

// nro of threads
#define NUM_THREADS 4

#define nboard 2
#define ylong 20
#define xlong 40


// boards

//structs 
struct job {
    //parcial board
    int partial_board [ylong][xlong];
    int x_start, x_end;
    int y_start, y_end;
    int id;  
};

// types
typedef void*       void_t;
typedef struct job* job_t;

int                 board[MAX_Y][MAX_X];
pthread_t           threads[NUM_THREADS];
job_t               jobs[NUM_THREADS];
pthread_barrier_t   barr;
void_t              status;

map_t               topology;

job_t               job_new();
void                job_show(job_t j);
void                job_pull(job_t j);
void                job_push(job_t j);
void                job_next(job_t j, int y, int x, int up, 
                             int dw, int rt, int lt, int e1, 
                             int e2, int e3, int e4); 
void                job_evolution(job_t j);
void_t              job_run(void_t j);


