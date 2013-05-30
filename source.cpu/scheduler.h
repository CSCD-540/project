#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define MAXPRO 6 // max num of processes
#define TIMEOUT_OPEN 50 //use for opening timeout
#define TIMEOUT_RW 30 //use for read write timeout

struct node {
	int pidnumber;
	int timeout;
	struct node *next;
};

struct queue {
	struct node *first; //points to the first node
	struct node *last; //points to the last node
	int size; //actual size of queue
};

struct queue suspended;

extern int scheduler_getPID(); //returns the pid of the fist node in the queue
extern int scheduler_hasNext(); //checks to see if there is a next node and returns 1 if true and 0 if false
extern void scheduler_nextProcess(int pid, int downtime); //revamp this one
extern void scheduler_timedown(); //need to revamp
extern void scheduler_removeProcess(struct queue *que, struct node *prev, struct node *cur); //removes specified node from queue
extern void scheduler_addToQueue(int pid, int downtime); //add to front of queue
extern void scheduler_remove(); //removes the first node








      
      
      

      
      
      
      
      
      

