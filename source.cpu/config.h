#ifndef CONFIG_H_INCLUDED
  #include <stdlib.h>
  #include <stdio.h>

  #define CONFIG_H_INCLUDED
  
  #define DEBUG 0

  #define MAXPRO           6  // max num of processes
  #define MAXMEM          16  // max size of a process
  #define STACKSIZE      100  // max size of the stack
  #define REGISTERSIZE    10  // size of each process registers
  #define MAXGMEM         20  // max size of global memory

  #define NORMAL           0  // denotes a normal return from exe()
  #define LOCKED           1  // stops process switching until unlock
  #define UNLOCKED         2  // remove lock
  #define ENDPROCESS       3
  #define p0WRITE          4  // tells p0 to run-p0 should only run after a write to gmem

  #define FS_DEBUG         0
  #define FS_VERBOSE       0  
  #define FS_NULL         -1
  #define FS_SIZE        400
  #define FS_COLUMNS      20

  #define PT_DEBUG         0
  #define PT_VERBOSE       0
  #define PT_NULL         -1
  #define PAGE_SIZE        4
  #define PT_MISS         -1
  
  #define SC_READY        -1
  #define SC_WAITING       1
  #define SC_DONE_WAITING  2
  
  #define SC_READ_TIME   100
  #define SC_WRITE_TIME  200
  
  #define FILE_PATH   "./programs.cpu/"
  

  #define FALSE 0
  #define TRUE 1
  
  char getChar(FILE* fp);
  char* getString(FILE* fp);
  int getInt(FILE* fp);

  void heavyLine();
  void lightLine();
  void starLine();
  
  void dumpData(int size, int* data);
#endif