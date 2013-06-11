#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>

#include <stdio.h>
#include <string.h>
 
#define SM_SIZE     1024
#define SM_KEY      5656

#define TRUE  1
#define FALSE 0

typedef char* String;