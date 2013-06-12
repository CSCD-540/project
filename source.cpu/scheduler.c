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
	int has;
	
	if(suspended.size > 0)
		has = 1;
	else
		has = 0;
	
	return has;
} //end of hasNext

void scheduler_nextProcess(int pid, int downtime) {
		scheduler_addToQueue(pid, downtime);
		

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
	
	printf("\nSize of suspended queue: %d\n", suspended.size);
	
	if(scheduler_hasNext() == 1) {
		printf("Inside if statement\n");
		while(cur != NULL) {
			printf("pid: %d has: %d time left\n", cur->pidnumber, cur->timeout);
			cur->timeout--;
			
			printf("time left: %d\n", cur->timeout);
			
			if(cur->timeout <= 0) {
				int x = cur->pidnumber;
				
				printf("removing from suspended queue\n");
				
				if(prev == NULL) {
					scheduler_remove();
				}
				else {
					scheduler_removeProcess(&suspended, prev, cur);
				}
			}
			prev = cur;
			cur = cur->next;
		}
		printf("Size of suspended queue: %d\n\n", suspended.size);
	}
	else {
		printf("\nNot going into if statement\n");
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
	
	newnode->pidnumber = pid;
	newnode->timeout = downtime;
	newnode->next = NULL;
	
	suspended.size++;
	
	if(suspended.first == NULL) {
		suspended.first = suspended.last = newnode; //adding the first node if list is empty
	}
	else {
		suspended.last->next = newnode; //adding newnode after last node
		suspended.last = suspended.last->next; //pointing "last" to last node
	}
} //end of addToQueue

void scheduler_remove() {
	struct node *temp;
	
	temp = suspended.first; //temp getting the first node
	
	suspended.first = suspended.first->next; //first node moving to the next one
	
	suspended.size--;
	
	free(temp);
} //end of remove

