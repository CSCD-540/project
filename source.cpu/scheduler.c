#include "scheduler.h"


/*
 * TODO:
 * add I/O interupt
 * all I need to do is add the I/O interrupt and put process
 * into a sleep or suspended mode for a set time and increment
 * down from that until time is 0 then pull out of sleep or
 * suspended and put back into process queue.
 * 
 * how many nodes do I have in suspended queue
 * 
 */

/*
 * returns the pid number of the first node in the queue.
 */
int scheduler_getPID() {
	return suspended.first->pidnumber;
} //end of getPID

/*
 * checks to see if there is another node, if true returns 1,
 * if false returns 0
 */
int scheduler_hasNext() {
	int has = 0;
	
	if(suspended.size > 0)
		has = 1;
	
	return has;
} //end of hasNext

void scheduler_nextProcess(int pid, int downtime) {
		scheduler_addToQueue(pid, downtime);
		
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
} //end of nextProcess

/*
 * time() should be going through the suspened queue
 * and decreasing the timeout of each process in the queue.
 * If timeout reaches 0 then it moves it back to the cpu.
 */ 
void scheduler_timedown() {
	struct node *cur = malloc(sizeof(struct node));
	struct node *prev = NULL;
	cur = suspended.first;
	
	if(scheduler_hasNext() == 1) {
		while(cur != NULL) {
			cur->timeout--;
			
			printf("\ntime left: %d\n", cur->timeout);
			
			if(cur->timeout == 0) {
				int x = cur->pidnumber;
				
				printf("\nremoving from suspended queue\n");
				
				if(prev == NULL) {
					scheduler_remove();
				}
				else {
					scheduler_removeProcess(&suspended, prev, cur);
				}
				//TODO: send back to cpu
			}
			prev = cur;
			cur = cur->next;
		}
	}
} //end of timedown

void scheduler_removeProcess(struct queue *que, struct node *prev, struct node *cur) {
	struct node *temp;
	temp = cur;
	prev->next = cur->next;
	que->size--;
	free(temp);
} //end of removeProcess

void scheduler_addToQueue(int pid, int downtime) {
	struct node *newnode = malloc(sizeof(struct node));
	struct queue x = suspended;
	
	newnode->pidnumber = pid;
	newnode->timeout = downtime;
	newnode->next = NULL;
	
	x.size++;
	
	if(x.first == NULL) {
		x.first = x.last = newnode; //adding the first node if list is empty
	}
	else {
		x.last->next = newnode; //adding newnode after last node
		x.last = x.last->next; //pointing "last" to last node
	}
} //end of addToQueue

void scheduler_remove() {
	struct node *temp;
	struct queue x = suspended;
	
	temp = x.first; //temp getting the first node
	
	x.first = x.first->next; //first node moving to the next one
	
	x.size--;
	
	free(temp);
} //end of remove

