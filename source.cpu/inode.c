#include "inode.h"

INode* inode_create(int process, int id, char* name, int start, int size) {
  INode* inode = calloc(1, sizeof(INode));
  inode->process = process;
  inode->id = id;
  inode->name = name;
  inode->start = start;
  inode->size = size;
  
  return inode;
}

void inode_destroy(INode* inode) {
  free(inode);
}

void inode_print(void* value) {
  INode* inode = (INode*)value;

  if (value == NULL)
    return;
  
  printf("  process: %4d id: %4d \t start: %4d \t size: %4d \t name: %-16s \n", inode->process, inode->id, inode->start, inode->size, inode->name);
}

int inode_compareByProcess(void* valueA, void* valueB) {
  INode* inodeA  = (INode*)valueA;
  int    process = (int)valueB;

  return inodeA->process - process;
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