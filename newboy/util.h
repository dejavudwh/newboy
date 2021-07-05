#ifndef __NEWBOY_UTIL_H__
#define __NEWBOY_UTIL_H__

#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <sys/syscall.h>

namespace newboy {

    pid_t GetThreadID();
    uint32_t GetFiberId();
}

#endif