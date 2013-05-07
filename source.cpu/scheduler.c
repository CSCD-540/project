#include <stdio.h>

#define MAXPRO 6;

int process;

void scheduler_nextProcess() {
	if(process + 1 > MAXPRO)
		process = 0;
	else
		process++;
}
