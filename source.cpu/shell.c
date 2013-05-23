#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "sharedmemory.h"

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

    if (strncmp(line, "q", 1) == 0)
      loop = FALSE;
      
  	// writes user input string to memory
    strcpy(sm_space, line);
    
    // wait for CPU to respond
    usleep(1000);
      
    // display response from CPU
    printf("received: %s", sm_space);
    
  }
	
/*
	while (1) {
		printf("<3 ");


		if(!fgets(line, MAX_LENGTH, stdin)) break;
		
		
		
/* temporarily commented out to implement shared memory		
		//parse and execute command
		if((cmd = strtok(line, DELIMS))) {
			//clear errors
			errno = 0;
			
			if(strcmp(cmd, "cd") == 0) {
				char *arg = strtok(0, DELIMS);
			
				if(!arg) {
					fprintf(stderr, "Missing argument; plese include name of directory.\n");
				} //end if
				else {
					//chdir(arg);
					//result = changeDir(arg);
					changeDir(arg);
					if (result != 0) {
						//if something goes wrong with changing directories; can be modified to deal with different return values.
						printf("Error in changing directory; please try again.\n");
					} //end if
				} //end else
			} //end if
			else if(strcmp(cmd, "exit") == 0) { //exit the shell, should also trigger the shutdown proceedures for the OS.
				break;
			} //end else if
			else {
				system(line);
			}
			
			if(errno) {
				perror("Command Failed");
			}
		} //end if

	} //end while
*/	
	// release the shared memory
  if(shmdt(sm_space) != 0)
    return -3;
	
	return 0;
} //end main

//change to the directory asked for by the user
int changeDir() {
	
} //end changeDir

//list all the contents of the current working directory.
int listDir() {
	
} //end listDir

//Extra features: http://cnswww.cns.cwru.edu/php/chet/readline/rltop.html
void readLine() {
	
} //end readLine

void history() {
	
} //end history