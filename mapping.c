#include "mapping.h"

map_t map_discover()
{

    map_t m = (map_t) malloc (sizeof(struct map)); 
    hwloc_topology_init(&m->topology);
    hwloc_topology_load(m->topology);
    m->topodepth = hwloc_topology_get_depth(m->topology);
    m->coredepth = hwloc_get_type_or_below_depth(m->topology, HWLOC_OBJ_CORE);
}


int map_corebind(map_t m, int cpu) 
{

    hwloc_obj_t obj = hwloc_get_obj_by_depth(m->topology, m->coredepth, 0 );
    if (obj) {
        m->cpuset = hwloc_bitmap_dup(obj->cpuset);
        hwloc_bitmap_singlify(m->cpuset);
        if (hwloc_set_cpubind(m->topology, m->cpuset, cpu)) {
            char *str;
            int error = errno;
            hwloc_bitmap_asprintf(&str, obj->cpuset);
            printf("Couldn't bind to cpuset %s: %s\n", str, strerror(error));
            free(str);
        }
    }
}

int map_ncores(map_t m) 
{
    return 4;//hwloc_get_nbobjs_by_depth(m->topology, m->coredepth) ;
}

int map_destroy(map_t m) 
{
    hwloc_topology_destroy(m->topology);
    free(m);
    return 1;
}

