#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "config.h"
#include "sharedmemory.h"
#include "shell.h"

#define MAX_LENGTH 1024
#define DELIMS " \t\r\n"

int main() {
  /* shared memory - Thanks to Andrew Canfield*/
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
	    if(SH_DEBUG)
			printf("line: %s \n", line);

	    if(strncmp(line, "ls", 2) == 0) {
	    	shell_ls(line, sm_space);
	    }
	    else if(strncmp(line, "open", 4) == 0) {
	    	shell_open(line, sm_space);
	    }
	    else if(strncmp(line, "rm", 2) == 0) {
	    	shell_rm(line, sm_space);
	    }
	    else if(strncmp(line, "copy", 4) == 0) {
	    	shell_copy(line, sm_space);
	    }
	    else if(strncmp(line, "import", 6) == 0) {
	    	shell_import(line, sm_space);
	    }
	    else if(strncmp(line, "load", 4) == 0) {
	    	shell_load(line, sm_space);
	    }
	    else if(strncmp(line, "run", 3) == 0) {
	    	shell_run(line, sm_space);
	    }
	    else if(strncmp(line, "dump", 4) == 0) {
	    	shell_dump(line, sm_space);
	    }
	    else if(strncmp(line, "cat", 3) == 0) {
	    	shell_cat(line, sm_space);
	    }
	    else if(strncmp(line, "help", 4) == 0) {
	    	shell_help(line, sm_space);
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
		    printf("> %s\n", sm_space);
		}
	} //end while

	// release the shared memory
  if(shmdt(sm_space) != 0)
    return -3;
	
	return 0;
} //end main

void shell_ls(char line[], char* sm_space) {
	// writes user input string to memory
    strcpy(sm_space, line);
    
    // wait for CPU to respond
    usleep(1000);

    // display response from CPU
    printf("> %s\n", sm_space);
} //end ls

void shell_open(char line[], char* sm_space) {
	// writes user input string to memory
    strcpy(sm_space, line);
    
    // wait for CPU to respond
    usleep(1000);

    // display response from CPU
    printf("> %s\n", sm_space);
} //end open

void shell_import(char line[], char* sm_space) {
	// writes user input string to memory
    strcpy(sm_space, line);
    
    // wait for CPU to respond
    usleep(1000);

    // display response from CPU
    printf("> %s\n", sm_space);
} //end open

void shell_rm(char line[], char* sm_space) {
	// writes user input string to memory
    strcpy(sm_space, line);
    
    // wait for CPU to respond
    usleep(1000);

    // display response from CPU
    printf("> %s\n", sm_space);
} //end rm

void shell_copy(char line[], char* sm_space) {
	// writes user input string to memory
    strcpy(sm_space, line);
    
    // wait for CPU to respond
    usleep(1000);

    // display response from CPU
    printf("> %s\n", sm_space);
} //end copy

void shell_load(char line[], char* sm_space) {
	// writes user input string to memory
    strcpy(sm_space, line);
    
    // wait for CPU to respond
    usleep(1000);

    // display response from CPU
    printf("> %s\n", sm_space);
} //end load

void shell_run(char line[], char* sm_space) {
	// writes user input string to memory
    strcpy(sm_space, line);
    
    // wait for CPU to respond
    usleep(1000);

    // display response from CPU
    printf("> %s\n", sm_space);
} //end run

void shell_dump(char line[], char* sm_space) {
	// writes user input string to memory
    strcpy(sm_space, line);
    
    // wait for CPU to respond
    usleep(1000);

    // display response from CPU
    printf("> %s\n", sm_space);
} //end dump

void shell_cat(char line[], char* sm_space) {
	// writes user input string to memory
    strcpy(sm_space, line);
    
    // wait for CPU to respond
    usleep(1000);

    // display response from CPU
    printf("> %s\n", sm_space);
} //end cat

void shell_help(char line[], char* sm_space) {
	// writes user input string to memory
    strcpy(sm_space, line);
    
    // wait for CPU to respond
    usleep(1000);

    // display response from CPU
    printf("> %s\n", sm_space);
} //end help