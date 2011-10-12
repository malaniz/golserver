#define _XOPEN_SOURCE 600
#include <math.h>
#include "job.h"

job_t*               jobs;


void globals_init(int width, int height, int generations)
{
    int i;

    GENERATIONS = generations;
    MAX_Y = height;
    MAX_X = width;

    topology = map_discover();
    NUM_THREADS = map_nthreads(topology);
    //nboard = (int)( log(NUM_THREADS+1) / log(2)); 
    //ylong  = MAX_Y / nboard;

    nboard = 1;
    printf("%d\n", NUM_THREADS);
    ylong  = MAX_Y / NUM_THREADS;
    xlong  = MAX_X / nboard;
    printf ("ylong: %d, xlong: %d \n", ylong, xlong);

    board = malloc (sizeof(int*)*MAX_Y);
    for (i=0; i< MAX_Y; i++) {
        board[i] = malloc(sizeof(int)* MAX_X);
    }
    threads  = malloc (sizeof(pthread_t )*NUM_THREADS);
    jobs     = malloc (sizeof(job_t     )*NUM_THREADS);
    measures = malloc (sizeof(measure_t)*NUM_THREADS);
    for (i=0; i< NUM_THREADS; i++) {
      measures[i] = measure_new();
    }
}

void globals_destroy()
{
    int i;

    for (i=0; i< MAX_Y; i++) {
        free(board[i]);
    }
    free(board);
    
    free(threads);
    free(jobs);
    free(measures);
    //destroy topology
}


void board_init()
{
    int y, x;
    unsigned int iseed = (unsigned int)time(NULL);
    srand(iseed);

    for (y=0; y<MAX_Y; y++) {
        for (x=0; x<MAX_X; x++) {
            board[y][x] = rand() > (RAND_MAX /1.3)? 1: 0;
        }
    }
}


void init_jobs()
{
    int cx, cy, pos, i;

    //printf("%d\n", nboard);
    /*
    pos = 0;
    for (cy=0; cy<nboard; cy++) {
        for (cx=0; cx<nboard; cx++) {
            jobs[pos] = job_new();
            jobs[pos]->id = pos;
            jobs[pos]->x_start = 0 * xlong;
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
    */


    printf("Entrando: threads %d \n", NUM_THREADS);
    for (i=0; i<NUM_THREADS; i++) {
            jobs[i] = job_new();
            jobs[i]->id = i;
            jobs[i]->x_start = 0 ;
            jobs[i]->x_end   = xlong;
            jobs[i]->y_start = i * ylong;
            jobs[i]->y_end   = jobs[i]->y_start + ylong;
            printf("job[%d] <xs:%d, xe:%d, ys:%d, ye:%d> \n", 
                jobs[i]->id, 
                jobs[i]->x_start,
                jobs[i]->x_end,  
                jobs[i]->y_start, 
                jobs[i]->y_end
            );
    }
}

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <stdio.h>


static int run(lua_State* L) 
{
    int i, rc, sizeparams, width, height, viewer, generations;

    sizeparams = lua_gettop(L);
    viewer = 1;
    if (sizeparams < 1) {
        width = 80;
        height = 40;
        generations = 100;
    } else if (sizeparams == 3) {
        height  = lua_tonumber(L, 1);
        width = lua_tonumber(L, 2);
        generations = lua_tonumber(L, 3);
    } else {
        lua_pushstring(L, "doit: 3 or 0 parameters");
        lua_error(L);
    }

    globals_init(width, height, generations);
    board_init();

    init_jobs();
    // Barrier initialization
    //
    if(pthread_barrier_init(&barr, NULL, NUM_THREADS))
    {
        printf("Could not create a barrier\n");
        return -1;
    }
    for (i=0; i<NUM_THREADS; i++) { 
        jobs[i]->core = i;
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


static int statistics(lua_State* L)
{
    int i, top;

    if (measures[0]->val == 0 || 
        measures[1]->val == 0 || 
        measures[2]->val == 0 || 
        measures[3]->val == 0 ) {

        lua_pushnil(L);
    } else {
        lua_newtable(L);
        top = lua_gettop(L);
        for (i = 0;i<NUM_THREADS; i++) {
            printf("thread %d: %s\n", i, measures[i]->__str__);
            lua_pushnumber(L, i);
            lua_pushnumber(L, measures[i]->val);
            lua_settable(L, top);
        }
    }
    return 1;
}


/*
static int board_str(lua_State* L)
{
    int y, x;
    char *s = (char *) malloc (sizeof(char)*MAX_X*3);
    char *t = (char *) malloc (sizeof(char)*MAX_Y*MAX_X*4);
    sprintf (t, "", "");
    for (y=0; y<MAX_Y; y++) {
        sprintf (s, "", "");
        for (x=0; x<MAX_X; x++) {
            if (board[y][x] == 1){ sprintf(s, "%s,1", s); }
            else                 { sprintf(s, "%s,0", s); }
        }
        s[0] = '[';
        sprintf (s, "%s]", s);
        if (y == 0) {
            sprintf (t, "{ matrix: [ %s", s);
        } else if ( y == (MAX_Y -1)) {
            sprintf (t, "%s, %s ] }", t, s); 
        } else {
            sprintf (t, "%s,%s",  t, s);
        }
    }
    lua_pushstring(L, t);
    return 1;
}
*/

int luaopen_golpthread(lua_State *L) 
{
    lua_register(L, "doit", run);
    lua_register(L, "data", board_str);
    lua_register(L, "stats", statistics);
    return 0;
}

