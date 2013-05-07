#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_LENGTH 1024
#define DELIMS " \t\r\n"

int main() {
	char *cmd;
	char line[MAX_LENGTH];
	int result;
	
	while (1) {
		printf("<3 ");
		if(!fgets(line, MAX_LENGTH, stdin)) break;
		
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
					result = changeDir(arg);
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