#include "sharedmemory.h"
 
main(int argc, char **argv) {
  int    loop;
	int    sm_id;

  String sm_space;

  // create the segment and set permissions
	if ((sm_id = shmget(SM_KEY, SM_SIZE, IPC_CREAT | 0666)) < 0) 
		return -1;
	
  // attach the segment to our data space
	if ((sm_space = shmat(sm_id, NULL, 0)) == (char *) -1) 
		return -2;
 
  // clear the memory
	memset(sm_space, 0, SM_SIZE);

  strcpy(sm_space, "+CPU_WAITING+");
  
	// read user input from client and print to screen
  loop = TRUE;
	while (loop) {

	  if (strcmp(sm_space, "+CPU_WAITING+") != 0) {
	    
	    if (strncmp(sm_space, "q", 1) == 0) {
        loop = FALSE;
        printf("received: %s", sm_space);
      }
      else if(strncmp(sm_space, "ls", 1) == 0) {
      	// initial file listing
		    char* s;

		    printf("file listing: \n%s\n", s);
		    strcpy(sm_space, s);
      } //end if
      //template
      else if(strncmp(sm_space, "open", 4) == 0) {
        printf("Opening File...\n");

        //code to open file in the cpu

        // make response
        strcpy(sm_space, "results of opening!\n");

        usleep(1500);
      
        // resume loop
        strcpy(sm_space, "+CPU_WAITING+");

      }
      else if(strncmp(sm_space, "import", 6) == 0) {
        printf("Importing File...\n");

        //code to import file in the cpu

        // make response
        strcpy(sm_space, "results of import!\n");

        usleep(1500);
      
        // resume loop
        strcpy(sm_space, "+CPU_WAITING+");

      }
      else {
      	// make response
      	strcpy(sm_space, "response from CPU\n");

        usleep(1500);
      
      	// resume loop
      	strcpy(sm_space, "+CPU_WAITING+");
      } //end else
    }		

	}
 
	if(shmdt(sm_space) != 0)
    return -3;
    
	return 0;
}