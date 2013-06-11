#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "sharedmemory.h"
#include "shell.h"

#define MAX_LENGTH 1024
#define DELIMS " \t\r\n"

int main() {
  /* shared memory */
  int loop;
  int   sm_id;
  char* sm_space;
  
  // locate the memory segment
  if ((sm_id = shmget(SM_KEY, SM_SIZE, 0666)) < 0)
    return -1;
  
  // attache the segment to the data space
  if ((sm_space = shmat(sm_id, NULL, 0)) == (char*) -1)
    return -2;
    
  /* end shared memory */
  
	char *cmd;
	char line[MAX_LENGTH];
	int  result;
	
  	loop = TRUE;
	
	while(loop) {
	    printf("> ");
	    fgets(line, MAX_LENGTH, stdin);
	    printf("line: %s \n", line);

	    //basic template
	    if(strncmp(line, "open", 4) == 0) {
	    	shell_openFile(line, sm_space);
	    }
	    else if(strncmp(line, "import", 6) == 0) {
	    	shell_importFile(line, sm_space);
	    }
	    else if (strncmp(line, "q", 1) == 0) {
	    	loop = FALSE;
	    	strcpy(sm_space, line);
	    }
	    else {
		  	// writes user input string to memory
		    strcpy(sm_space, line);
		    
		    // wait for CPU to respond
		    usleep(1000);
		      
		    // display response from CPU
		    printf("received: %s \n", sm_space);
		}
	} //end while

	// release the shared memory
  if(shmdt(sm_space) != 0)
    return -3;
	
	return 0;
} //end main

void shell_openFile(char line[], char* sm_space) {

	//for testing
	// writes user input string to memory
    strcpy(sm_space, line);
    
    // wait for CPU to respond
    usleep(1000);

    // display response from CPU
    printf("received: %s", sm_space);
}

void shell_importFile(char line[], char* sm_space) {


	//for testing
	// writes user input string to memory
    strcpy(sm_space, line);
    
    // wait for CPU to respond
    usleep(1000);

    // display response from CPU
    printf("received: %s", sm_space);
}