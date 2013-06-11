#include "scheduler.h"
 
 void scheduler_init(int pid) {
	int i;
	for(i = 0; i < pid; i++) { //init the number of pids to READY
		wait_time[i] = READY;
		wait_state[i] = READY;
	}
}//end init

int scheduler_nextProcess(int pid) {
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

} //end of nextProcess
