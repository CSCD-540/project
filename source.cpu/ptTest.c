#include "filesystem2.h"
#include "pagetable.h"

extern int mem[MAXPRO][MAXMEM];
extern int currentProgramId;

int main() {
  int i;
  int j;
  
  int currentProcess;
  
  heavyLine();
  printf("filesystem self test\n");
  heavyLine();
  printf("\n");
  
  // initialize the filesystem and the pagetable
  fs_initialize();
  pt_initialize();
  
  // import programs
  fs_import("./programs.cpu/prog1out.cpu", "prog1");
  fs_nodeList();
  fs_dump();
  
  pt_dump();

  // set current program (global)
  currentProgramId = 1;
  currentProcess = 0;
  
  for (i = 0; i < fs_getProcessSize(currentProgramId, currentProcess); i++) {
    printf("instruction: %d \n", pt_getInstruction(currentProcess, i));
  }
  
  starLine();
  currentProcess = 1;
  for (i = 0; i < fs_getProcessSize(currentProgramId, currentProcess); i++) {
    printf("instruction: %d \n", pt_getInstruction(currentProcess, i));
  }
  
  starLine();
  currentProcess = 2;
  for (i = 0; i < fs_getProcessSize(currentProgramId, currentProcess); i++) {
    printf("instruction: %d \n", pt_getInstruction(currentProcess, i));
  }
  
  pt_dump();
  
  return 0;
}