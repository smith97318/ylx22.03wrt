#include "QMIThread.h"
#include <sys/time.h>

#if defined(__STDC__)
#include <stdarg.h>
#define __V(x)	x
#else
#include <varargs.h>
#define __V(x)	(va_alist) va_dcl
#define const
#define volatile
#endif

#ifdef ANDROID
#define LOG_TAG "NDIS"
#include "../ql-log.h"
#else
#include <syslog.h>
#endif

int debug = 0;

#ifndef ANDROID //defined in atchannel.c
static void setTimespecRelative(struct timespec *p_ts, long long msec)
{
    struct timeval tv;

    gettimeofday(&tv, (struct timezone *) NULL);

    /* what's really funny about this is that I know
       pthread_cond_timedwait just turns around and makes this
       a relative time again */
    p_ts->tv_sec = tv.tv_sec + (msec / 1000);
    p_ts->tv_nsec = (tv.tv_usec + (msec % 1000) * 1000L ) * 1000L;
}

int pthread_cond_timeout_np(pthread_cond_t *cond,
                            pthread_mutex_t * mutex,
                            unsigned msecs) {
    if (msecs != 0) {
        struct timespec ts;
        setTimespecRelative(&ts, msecs);
        return pthread_cond_timedwait(cond, mutex, &ts);
    } else {
        return pthread_cond_wait(cond, mutex);
    }
}
#endif

static const char * get_time(void) {
    static char time_buf[68] = {0};
    struct timeval  tv;
    time_t time;
    suseconds_t millitm;
    struct tm *ti;

    gettimeofday (&tv, NULL);

    time= tv.tv_sec;
    millitm = (tv.tv_usec + 500) / 1000;

    if (millitm == 1000) {
        ++time;
        millitm = 0;
    }

    ti = localtime(&time);
    sprintf(time_buf, "[%02d-%02d_%02d:%02d:%02d:%03d]", ti->tm_mon+1, ti->tm_mday, ti->tm_hour, ti->tm_min, ti->tm_sec, (int)millitm);
    return time_buf;
}

FILE *logfilefp = NULL;
static pthread_mutex_t printfMutex = PTHREAD_MUTEX_INITIALIZER;
static char line[1024];
void dbg_time (const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    pthread_mutex_lock(&printfMutex);
#ifdef ANDROID
    vsnprintf(line, sizeof(line), fmt, args);
    RLOGD("%s", line);
#else
#ifdef LINUX_RIL_SHLIB
    line[0] = '\0';
#else
    snprintf(line, sizeof(line), "%s ", get_time());
#endif
    vsnprintf(line + strlen(line), sizeof(line) - strlen(line), fmt, args);
    fprintf(stdout, "%s\n", line);
#endif
    if (logfilefp) {
        fprintf(logfilefp, "%s\n", line);
    }
    pthread_mutex_unlock(&printfMutex);
}

const int i = 1;
#define is_bigendian() ( (*(char*)&i) == 0 )

USHORT le16_to_cpu(USHORT v16) {
    USHORT tmp = v16;
    if (is_bigendian()) {
        unsigned char *s = (unsigned char *)(&v16);
        unsigned char *d = (unsigned char *)(&tmp);
        d[0] = s[1];
        d[1] = s[0];
    }
    return tmp;
}

UINT  le32_to_cpu (UINT v32) {
    UINT tmp = v32;
    if (is_bigendian()) {
        unsigned char *s = (unsigned char *)(&v32);
        unsigned char *d = (unsigned char *)(&tmp);
        d[0] = s[3];
        d[1] = s[2];
        d[2] = s[1];
        d[3] = s[0];
    }
    return tmp;
}

UINT ql_swap32(UINT v32) {
    UINT tmp = v32;
    {
        unsigned char *s = (unsigned char *)(&v32);
        unsigned char *d = (unsigned char *)(&tmp);
        d[0] = s[3];
        d[1] = s[2];
        d[2] = s[1];
        d[3] = s[0];
    }
    return tmp;
}

USHORT cpu_to_le16(USHORT v16) {
    USHORT tmp = v16;
    if (is_bigendian()) {
        unsigned char *s = (unsigned char *)(&v16);
        unsigned char *d = (unsigned char *)(&tmp);
        d[0] = s[1];
        d[1] = s[0];
    }
    return tmp;
}

UINT cpu_to_le32 (UINT v32) {
    UINT tmp = v32;
    if (is_bigendian()) {
        unsigned char *s = (unsigned char *)(&v32);
        unsigned char *d = (unsigned char *)(&tmp);
        d[0] = s[3];
        d[1] = s[2];
        d[2] = s[1];
        d[3] = s[0];
    }
    return tmp;
}
