#include <hwloc.h>
#include <stdlib.h>
#include <stdio.h>

struct map {
    int topodepth;
    int coredepth;
    int threaddepth;
    hwloc_topology_t topology;
    hwloc_cpuset_t cpuset;
};
typedef struct map* map_t;


map_t map_discover();
int   map_corebind(map_t, int);
int   map_destroy (map_t);
int   map_ncores(map_t);
