#include "inode.h"

INode* inode_create(int id, char* name, int fileStart, int fileSize, int processes, int processStart[MAXPRO], int processSize[MAXPRO]) {
  int i;

  INode* inode = calloc(1, sizeof(INode));
  inode->id         = id;
  inode->name       = name;
  inode->processes  = processes;
  inode->fileStart  = fileStart;
  inode->fileSize   = fileSize;
  
  for (i = 0; i < MAXPRO; i++)
    inode->processStart[i] = processStart[i];

  for (i = 0; i < MAXPRO; i++)
    inode->processSize[i] = processSize[i];
  
  return inode;
}

void inode_destroy(INode* inode) {
  free(inode);
}

void inode_print(void* value) {
  int i;
  INode* inode = (INode*)value;

  if (value == NULL)
    return;
  
  printf("  id: %4d  name: %-16s fileSize: %4d  fileStart: %4d  processes: %d \n", inode->id, inode->name, inode->fileSize, inode->fileStart, inode->processes);
  if (FS_VERBOSE) {
    printf("    processSize: [");
    for (i = 0; i < inode->processes; i++)
      printf("%d ", inode->processSize[i]);
    printf("]\n    processStart: [ ");
    for (i = 0; i < inode->processes; i++)
      printf("%d ", inode->processStart[i]);
    printf("] \n");
  }
  
  

}

int inode_compareById(void* valueA, void* valueB) {
  INode* inodeA = (INode*)valueA;
  int    id     = (int)valueB;

  return inodeA->id - id;
}

int inode_compareByName(void* valueA, void* valueB) {
  INode* inodeA = (INode*)valueA;
  
  return strcmp(inodeA->name, (char*)valueB);
}