#ifndef INODE_H_INCLUDED
  #define INODE_H_INCLUDED
  
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  
  #include "config.h"
  #include "list.h"
  
  typedef struct INode {
    int   id;
    char* name;
    int   fileSize;
    int   fileStart;
    int   processes;
    int   processStart[MAXPRO];
    int   processSize[MAXPRO];
  } INode;
  
  INode* inode_create(int id, char* name, int fileStart, int fileSize, int processes, int processStart[MAXPRO], int processSize[MAXPRO]);
  void inode_destroy(INode* inode);
  void inode_print(void* value);
  int inode_compareById(void* valueA, void* valueB);
  int inode_compareByName(void* valueA, void* valueB);
  
#endif