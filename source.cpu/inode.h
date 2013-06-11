#ifndef INODE_H_INCLUDED
  #define INODE_H_INCLUDED
  
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  
  #include "list.h"
  
  typedef struct INode {
    int   id;
    char* name;
    int   start;
    int   size;
  } INode;
  
  INode* inode_create(int id, char* name, int start, int size);
  void inode_destroy(INode* inode);
  void inode_print(void* value);
  int inode_compareById(void* valueA, void* valueB);
  
  
#endif