#include <time.h>
#include <sys/timeb.h>

#define SEC2NSEC((s) * 1000 * 1000 * 1000)
#define RESET_TMP()     clock_gettime(CLOCK_REALTIME, &time1)
#define SET_TMP()       clock_gettime(CLOCK_REALTIME, &time2)
#define SEC_TRANS()     (time2.tv_sec - time1.tv_sec)
#define NSEC_TRANS()    (time2.tv_nsec - time1.tv_nsec)

void diff_time( timespec *t_fin, timespec *t_ini, timespec *delta );
