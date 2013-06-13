#include "scheduler.h"

extern int wait_state[MAXPRO];
extern int wait_time[MAXPRO];

void scheduler_init() {
  int i; 
  
  for (i = 0; i < MAXPRO; i++) {
    wait_time[i] = SC_READY;
    wait_state[i] = SC_READY;
  }
}

int scheduler_nextProcess(int pid) {
  int cur_proc;

  do {
    cur_proc = (fs_getProcessCount(pid) == 1) ? 0 : rand() % (fs_getProcessCount(pid));
    
    
    if (wait_time[cur_proc] > 0) {
      lightLine();
      printf("process: %d is waiting for %d cycles.\n", cur_proc, wait_time[cur_proc]);
      lightLine();
      
      wait_time[cur_proc]--;
    }
	//Need the && or every time after it completes once it will enter the if
    if (wait_time[cur_proc] == 0 && wait_state[cur_proc] == SC_WAITING) {
      wait_state[cur_proc] = SC_DONE_WAITING;
      wait_time[cur_proc] = 0;
      heavyLine();
      printf("Process %d leaving wait state.\n", cur_proc);
      heavyLine();
    }
    
  } while (wait_state[cur_proc] == SC_WAITING);
  
  return cur_proc;
}

