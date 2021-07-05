#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <sys/syscall.h>

namespace newboy {

    pid_t GetThreadID();
    uint32_t GetFiberId();
}