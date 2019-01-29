#include <stdint.h>
#ifdef _MSC_VER

#include <windows.h>

int64_t ref_time_ticks () {
    LARGE_INTEGER  t0;
    QueryPerformanceCounter(&t0);
    return t0.QuadPart;
}

int get_time_usec ( int64_t reft ) {
    int64_t t0 = ref_time_ticks();
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    return (int)(((t0-reft)*1000000) / freq.QuadPart);
}

#elif __linux__

const uint64_t NSEC_IN_SEC = 1000000000LL;

int64_t ref_time_ticks () {
    timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) {
        assert(false);
        return -1;
    }

    return ts.tv_sec * NSEC_IN_SEC + ts.tv_nsec;
}

int get_time_usec ( int64_t reft ) {
    return (int) ((ref_time_ticks() - reft) / (NSEC_IN_SEC/1000000));
}

#else // __linux__

#include <mach/mach.h>
#include <mach/mach_time.h>

int64_t ref_time_ticks() {
    return mach_absolute_time();
}

int get_time_usec ( int64_t reft ) {
    int64_t relt = ref_time_ticks() - reft;
    mach_timebase_info_data_t s_timebase_info;
    mach_timebase_info(&s_timebase_info);
    return relt * s_timebase_info.numer/s_timebase_info.denom/1000;
}

#endif
