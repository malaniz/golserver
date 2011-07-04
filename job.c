#include "job.h"

job_t job_new()
{
    int i, k;
    job_t j = (job_t) malloc (sizeof(struct job)); 
    j->id = -1;
    return j;
}

void job_pull(job_t j) 
{
    int y, x,
        ys = j->y_start, ye = j->y_end,
        xs = j->x_start, xe = j->x_end;
    for (y=0; y < ylong; y++) {
        for (x = 0; x < xlong; x++) {
            j->partial_board[y][x] = board[ys+y][xs+x];
        }
    }
}

void job_push(job_t j) 
{
    int y, x,
        ys = j->y_start, ye = j->y_end,
        xs = j->x_start, xe = j->x_end;

    for (y=0; y < ylong; y++) {
        for (x = 0; x < xlong; x++) {
            board[ys+y][xs+x] = j->partial_board[y][x];
        }
    }
}


void job_next(job_t j, int y, int x, 
    int up, int dw, int rt, int lt, 
    int e1, int e2, int e3, int e4) 
{
    int xx = up + dw + rt + lt + e1 + e2+ e3 + e4 , 
        res=0;

    if (j->partial_board[y][x] == 0) {
        res = ( xx == 3 ) ? 1: 0;
    } else {
        res = ( xx < 2  )? 0:
              ( xx > 3  )? 0:
              ( xx == 2 )? 1:
              ( xx == 3 )? 1: -1;
    }
    j->partial_board[y][x] = res;
}


void job_evolution(job_t j)
{
    int x, y, 
        up, dw, rt, lt, 
        r_x, l_x, u_y, d_y, 
        e1, e2, e3, e4,
        ys = j->y_start, ye = j->y_end,
        xs = j->x_start, xe = j->x_end;

    job_pull(j);
    for (y=0; y< ylong; y++) {
        for (x=0; x<xlong ; x++) {
            // compute x neightbors as torus 
            // never go to the limit of board slice
            l_x = x - 1 % MAX_X;
            r_x = x + 1 % MAX_X;
            u_y = y - 1 % MAX_Y;
            d_y = y + 1 % MAX_Y;
            rt = board[y][r_x];
            lt = board[y][l_x];
            up = board[u_y][x];
            dw = board[d_y][x];
            e1 = board[u_y][l_x]; 
            e2 = board[u_y][r_x]; 
            e3 = board[d_y][r_x]; 
            e4 = board[d_y][l_x]; 
            job_next(j, y, x, up, dw, rt, lt, e1, e2, e3, e4); 
        }
    }
    // barrier 
     int rc = pthread_barrier_wait(&barr);
     if(rc != 0 && rc != PTHREAD_BARRIER_SERIAL_THREAD)
     {
         printf("Could not wait on barrier\n");
         exit(-1);
     }

    job_push(j);
}


void_t job_run(void_t _j)
{
    job_t j = (job_t)_j;
    int iterations = 9999;
    int c = 0;
    while(iterations){
        job_evolution(j);
        iterations--;
        usleep(100000);
        c = ( c + 1 ) % NUM_THREADS;
        //map_corebind(topology, c);
    }
    c = 0;

    pthread_exit(NULL);
}
