#include "config.h"
#include "filesystem2.h"

#define SC_READY -1
#define SC_WAITING 1
#define SC_DONE_WAITING 2
#define MAXPRO 6

int wait_state[MAXPRO];
int wait_time[MAXPRO];

void scheduler_init();
int scheduler_nextProcess(int pid);
