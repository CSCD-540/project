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
<<<<<<< HEAD
  int cur_proc;
  
  //int x = (rand() % (fs_getProcessCount(pid)));
  
  //printf("\n\nrandom number %d\n\n", x);
  
  do {
    cur_proc = (fs_getProcessCount(pid) == 1) ? 0 : (rand() % (fs_getProcessCount(pid)));
    
    
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
=======
	int cur_proc;
	
	do {
		cur_proc = (pid == 1) ? 0:(rand()%(pid - 1)) + 1; //get random process
		
		if(wait_time[cur_proc] > 0) {
			wait_time[cur_proc]--;
		}
		if(wait_time[cur_proc] == 0) {
			wait_state[cur_proc] = DONE_WAITING;
			wait_time[cur_proc] = READY; //Set wait_time[cur_proc] back to READY so that the process is delayed on read and write
			heavyLine();
			printf("Process %d leaving waiting state\n", cur_proc);
			heavyLine();
		}
	}while(wait_state[cur_proc] == WAITING); //if cur_proc state is WAITING go through the do while again
	
	return cur_proc;
>>>>>>> parent of dbaa0a9... filesystem now works equally well as filesystem2 for all progs except prog3, added debugging flags to makefile, all other changes should be minor

