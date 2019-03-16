#include "nanotiming.h"

void current_utc_time(struct timespec *ts) {

#ifdef __MACH__ // OS X does not have clock_gettime, use clock_get_time
    clock_serv_t cclock;
    mach_timespec_t mts;
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    ts->tv_sec = mts.tv_sec;
    ts->tv_nsec = mts.tv_nsec;
#else
    clock_gettime(CLOCK_REALTIME, ts);
#endif
}

static inline void unify_timedelta(struct timespec *ts)
{
    if (ts->tv_nsec < 0) {
        ts->tv_sec -= 1;
        ts->tv_nsec = 1000000000L + ts->tv_nsec;
    }
}

struct timespec benchmark(int (*func)(void *), void *arg) {
    struct timespec diff, start, end;
    int retval;
    current_utc_time(&start);
    // Call benchmark function
    retval = func(arg);
    current_utc_time(&end);
    diff.tv_sec = end.tv_sec - start.tv_sec;
    diff.tv_nsec = end.tv_nsec - start.tv_nsec;
    unify_timedelta(&diff);

    if (retval != 0) {
        printf("%s: warning: target returned %d.\n", __func__, retval);
    }
    return diff;
}

struct timespec benchmark_mt(int (*func)(void *), void *arg, unsigned int times) {
    struct timespec diff, start, end;
    unsigned int i;
    int retval = 0;
    current_utc_time(&start);
    // Call benchmark function
    for (i = 0; i < times && retval == 0; i ++) {
        retval = func(arg);
    }
    if (retval != 0) {
        printf("%s: warning: benchmark aborted in the middle as the target"
                "returned %d in the iteration %d.\n", __func__, retval,
                i);
    }
    current_utc_time(&end);
    diff.tv_sec = end.tv_sec - start.tv_sec;
    diff.tv_nsec = end.tv_nsec - start.tv_nsec;
    unify_timedelta(&diff);
    return diff;
}
