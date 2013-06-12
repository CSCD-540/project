#include "scheduler.h"

/*
 * TODO:
 * add I/O interupt
 * all I need to do is add the I/O interrupt and put process
 * into a sleep or suspended mode for a set time and increment
 * down from that until time is 0 then pull out of sleep or
 * suspended and put back into process queue.
 * 
 */
 
 void scheduler_init(int pid) {
	int i;
	for(i = 0; i < MAXPRO; i++) {
		wait_time[i] = READY;
		wait_state[i] = READY;
		//ready_proc[i] = 0;
	}
}//end init

int scheduler_nextProcess(int pid) {
	int cur_proc;
	
	do {
		cur_proc = (pid == 1) ? 0:(rand()%(pid - 1)) + 1;
		
		if(wait_time[cur_proc] > 0) {
			printf("cp: %d, wt: %d\n", cur_proc, wait_time[cur_proc]);
			wait_time[cur_proc]--;
		}
		if(wait_time[cur_proc] == 0) {
			wait_state[cur_proc] = DONE_WAITING;
		}
	}while(wait_state[cur_proc] == WAITING);
	

	
	return cur_proc;

} //end of nextProcess


//Old code will clean later


	/*int proc_rdy = 1;
	int i;
	
	cur_proc = (pid == 1) ? 0:(rand()%(pid - 1)) + 1;
	
	if(cur_proc == 0)
		printf("\n\npid is: %d\n\n", cur_proc);
	
	if(wait_time[cur_proc] == READY) {
		return cur_proc;
	}
	else {
		while(proc_rdy) {
			for(i = 1; i < pid; i++) {
				if(wait_time[i] > 0) {
					printf("pid %d wt: %d\n", i, wait_time[i]);
					wait_time[i]--;
				}
				else if(wait_time[i] == 0)
					ready_proc[i] = 1;
			}
			for(i = 1; i < pid; i++) {
				if(ready_proc[i] == 1)
					proc_rdy = 0;
			}
			
		}
		for(i = 1; i < pid; i++) {
			if(ready_proc[i] == 1)
				ready_proc[i] = 0;
				printf("proc: %d wt: %d\n", i, wait_time[i]);
				return ready_proc[i];
		}
	}*/




	/*cur_proc = (pid == 1) ? 0:(rand()%(pid - 1)) + 1;
	
	printf("\nInside nextProcess.  cur_proc is: %d\n", cur_proc);
	
	if(wait_time[cur_proc] == READY) {
		return cur_proc;
	}
	else {
		while(wait_time[cur_proc] > 0) {
			printf("Inside while\n");
			wait_time[cur_proc]--;
		
			printf("cur_proc: %d time left of cur_proc: %d\n", cur_proc, wait_time[cur_proc]);
		
			cur_proc = (pid == 1) ? 0:(rand()%(pid - 1)) + 1;
		}
	
		//wait_time[cur_proc] = -1;
	}
	
	return cur_proc;*/
		
	/*
	int x = downtime;
		
	struct timeval start, end;
		
	printf("\ngoing to sleep\n");
	gettimeofday(&start, 0);
	nanosleep(x, NULL);
	gettimeofday(&end, 0);
		
	long elapsed = (end.tv_sec - start.tv_sec)*1000000 + end.tv_usec-start.tv_usec;
	printf("\nprocess slept for %ld milliseconds\n", elapsed);
	*/
		
	/*
	 * TODO:
	 * Look into using nanosleep() to put the process to sleep for
	 * so many milliseconds.
	 * Or use sleep() to sleep for so many seconds.
	 */


