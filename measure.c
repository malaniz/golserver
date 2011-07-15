#include "measure.h"
#include <time.h>
#include <sys/timeb.h>

void diff_time( 
        struct timespec* t_fin, 
        struct timespec* t_ini, 
        struct timespec* delta )
{
    if( ( (*t_fin).tv_nsec - (*t_ini).tv_nsec ) < 0 ){
        if( (*t_fin).tv_sec == (*t_ini).tv_sec ){
            (*delta).tv_sec  = 0;
            (*delta).tv_nsec = 1000000000 + (*t_fin).tv_nsec - (*t_ini).tv_nsec;
        } else {
            (*delta).tv_sec  = (*t_fin).tv_sec - (*t_ini).tv_sec - 1;
            (*delta).tv_nsec = 1000000000 + (*t_fin).tv_nsec - (*t_ini).tv_nsec;
        }
    } else {
        if( (*t_fin).tv_sec == (*t_ini).tv_sec ) {
            (*delta).tv_sec  = 0;
            (*delta).tv_nsec = (*t_fin).tv_nsec - (*t_ini).tv_nsec;
        } else {
            (*delta).tv_sec  = (*t_fin).tv_sec - (*t_ini).tv_sec;
            (*delta).tv_nsec = (*t_fin).tv_nsec - (*t_ini).tv_nsec;
        }
    }
}

measure_t measure_new() 
{
    measure_t t =  malloc (sizeof(struct measure));
    sprintf (t->__str__, "TIME INVALID" );
    clock_gettime(CLOCK_REALTIME, &t->begin);
    return t;
}

char* time2str(time_t* t)
{
    return ctime(t);
}

measure_t measure_finish(measure_t t)
{
    unsigned long int b, e, d;
    clock_gettime(CLOCK_REALTIME, &t->end);
    diff_time(&t->end, &t->begin, &t->delta);
        
    b = t->begin.tv_sec*1000000000 + t->begin.tv_nsec;
    e = t->end.tv_sec  *1000000000 + t->end.tv_nsec;
    d = t->delta.tv_sec*1000000000 + t->delta.tv_nsec;

    sprintf(t->__str__, "<%lu, %lu, %lu>", b, e, d);

    return t;
}



