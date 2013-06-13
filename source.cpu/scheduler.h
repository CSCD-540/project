#include "config.h"
#include "filesystem2.h"

#define READY -1
#define WAITING 1
#define DONE_WAITING 2
#define MAXPRO 6

void scheduler_init();
int scheduler_nextProcess(int pid);
