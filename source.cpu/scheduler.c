#include <stdio.h>
#include <stdlib.h>

#define MAXPRO 6; //is in cpu.c however I have in for testing
#define TIMEOUT 30; //not used yet, will use for I/O time out

/*
 * TODO:
 * add I/O interupt
 * add round robin
 * add priority scheduling?
 * change all ready_queue to cpu queue
 */

//included for testing
struct node {
	int pidnumber;
	struct node *next;
};

//included for testing
struct queue {
	struct node *first; //points to the first node
	struct node *last; //points to the last node
	int size; //actual size of queue
};

//for testing purposes
struct queue ready_queue;
struct queue suspended;

int scheduler_getPID(struct queue *x) {
	return x->first->pidnumber;
} //end of getPID

//checks to see if there is another node, if true returns 1, if false returns 0
int scheduler_hasNext() {
	int has = 0;
	
	if(ready_queue.size > 0)
		has = 1;
	
	return has;
} //end of hasNext

int scheduler_nextProcess() {
	int x;
	if(ready_queue.first == NULL) {
		printf("No queue of processes\n");
		return -1;
	}
	else {
		x = scheduler_getPID(&ready_queue);
		scheduler_remove(&ready_queue);
		return x;
	}
} //end of nextProcess


//--------------------------Testing-----------------------------

//for testing
void scheduler_addToQueue(struct queue *x, int num) {
	struct node *newnode = malloc(sizeof(struct node));
	
	newnode->pidnumber = num;
	newnode->next = NULL;
	
	x->size++;
	
	if(x->first == NULL) {
		x->first = x->last = newnode; //adding the first node if list is empty
	}
	else {
		x->last->next = newnode; //adding newnode after last node
		x->last = x->last->next; //pointing "last" to last node
	}
} //end of addToQueue

//for testing
void scheduler_remove(struct queue *x) {
	struct node *temp;
	
	temp = x->first; //temp getting the first node
	
	x->first = x->first->next; //first node moving to the next one
	
	x->size--;
	
	free(temp);
} //end of remove

//filling ready_queue for testing
void fillQueue() {
	int i, x;
	x = MAXPRO;
	ready_queue.size = 0; //init size to 0
	for(i = 0; i < x; i++) {
		scheduler_addToQueue(&ready_queue, i + 2);
	}
} //end of fillQueue

//used for testing for now
main() {
	fillQueue();
	int x;
	x = scheduler_nextProcess();
	
	printf("%d\n", x);
	
	x = scheduler_nextProcess();
	
	printf("%d\n", x);
	
	scheduler_addToQueue(&ready_queue, 9);
	
	while(scheduler_hasNext() == 1) {
		x = scheduler_nextProcess();
		printf("%d\n", x);
	} //end of while
	
} //end of main
