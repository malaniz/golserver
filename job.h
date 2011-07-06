#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mapping.h"

// boards
#include "globals.h"

//structs 
struct job {
    //parcial board
    int* partial_board;
    int x_start, x_end;
    int y_start, y_end;
    int id;  
};

// types
typedef struct job* job_t;

void_t              status;


job_t               job_new();
void                job_show(job_t j);
void                job_pull(job_t j);
void                job_push(job_t j);
void                job_next(job_t j, int y, int x, int up, 
                             int dw, int rt, int lt, int e1, 
                             int e2, int e3, int e4); 
void                job_evolution(job_t j);
void_t              job_run(void_t j);


