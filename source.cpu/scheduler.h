#include "config.h"
#include "filesystem2.h"

#define SC_READY -1
#define SC_WAITING 1
#define SC_DONE_WAITING 2
#define MAXPRO 6

void scheduler_init();
int scheduler_nextProcess(int pid);
