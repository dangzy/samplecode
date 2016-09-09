#include <stdio.h>
#include <fcntl.h>
#define __USE_GNU
#include <dlfcn.h>
#include <stdarg.h>
#include <syslog.h>
static int open_cnt = 0;

int open(const char *pathname, int flags, ...) {
    int (*handler)(const char *,int,...) = NULL;
    va_list arg_ptr;
    int rc;
    int mode;
    openlog("hookopen",LOG_CONS|LOG_PID,LOG_USER);
    syslog(LOG_DEBUG,"hook open: %d\n",open_cnt++);
    closelog();
    handler = dlsym(RTLD_NEXT,"open");
    va_start(arg_ptr,flags);
    mode = va_arg(arg_ptr,int);
    if (mode)
    	rc = handler(pathname,flags,mode);
    else
        rc = handler(pathname,flags);
    va_end(arg_ptr);
    return rc;
}
