#ifndef H_UTILS
#define H_UTILS
#include <sys/time.h>
#include <unistd.h>
#include <string>
long getCurrentTime(){
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec*1000 + tv.tv_usec/100;
}


#endif
