#include <time.h>
#include <sys/timeb.h>
#include <stdlib.h>
#include <stdio.h>


struct measure {
    struct timespec begin;
    struct timespec end;
    struct timespec delta;
    char   __str__ [256];
    unsigned long int val;
};

typedef struct measure* measure_t;

measure_t measure_new();
measure_t measure_start(measure_t);
measure_t measure_finish(measure_t);
//measure_t measure_show(measure_t);

