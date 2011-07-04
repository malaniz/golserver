#define _XOPEN_SOURCE 600
#include "job.h"
/*
 * Problems with syncronization
 */

void board_init()
{
    int y, x;
    /* Simple "srand()" seed: just use "time()" */
    unsigned int iseed = (unsigned int)time(NULL);
    srand(iseed);

    for (y=0; y<MAX_Y; y++) {
        for (x=0; x<MAX_X; x++) {
            board[y][x] = rand() > (RAND_MAX /1.3)? 1: 0;
        }
    }
}


void board_show()
{
    int y, x;
    for (y=0; y<MAX_Y; y++) {
        for (x=0; x<MAX_X; x++) {
            if (board[y][x] == 1){ printf("#"); }
            else                 { printf("."); }
        }
        printf ("\n");
    }
}

void init_jobs()
{
    int cx, cy, pos;


    pos = 0;
    for (cy=0; cy<nboard; cy++) {
        for (cx=0; cx<nboard; cx++) {
            jobs[pos] = job_new();
            jobs[pos]->id = pos;
            jobs[pos]->x_start = cx * xlong;
            jobs[pos]->x_end   = jobs[pos]->x_start + xlong;
            jobs[pos]->y_start = cy * ylong;
            jobs[pos]->y_end   = jobs[pos]->y_start + ylong;
            printf("job[%d] <xs:%d, xe:%d, ys:%d, ye:%d> \n", 
                jobs[pos]->id, 
                jobs[pos]->x_start,
                jobs[pos]->x_end,  
                jobs[pos]->y_start, 
                jobs[pos]->y_end
            );
            pos++;
        }
    }
}

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <stdio.h>


static int run(lua_State* L) 
{
    int i, rc;
    board_init();

    topology = map_discover();

    init_jobs();
    // Barrier initialization
    if(pthread_barrier_init(&barr, NULL, NUM_THREADS))
    {
        printf("Could not create a barrier\n");
        return -1;
    }
    for (i=0; i<NUM_THREADS; i++) { 
        rc = pthread_create(&threads[i], NULL, job_run, (void_t)jobs[i]); 
    }
    
    return 1;
}

static int board_str(lua_State* L)
{
    int y, x;
    char *s = (char *) malloc (sizeof(char)*100000);
    for (y=0; y<MAX_Y; y++) {
        for (x=0; x<MAX_X; x++) {
            if (board[y][x] == 1){ sprintf(s, "%s#", s); }
            else                 { sprintf(s, "%s.", s); }
        }
        sprintf (s, "%s$", s);
    }
    lua_pushstring(L, s);
    return 1;
}

int luaopen_golpthread(lua_State *L) 
{
    lua_register(L, "doit", run);
    lua_register(L, "data", board_str);
    return 0;
}

