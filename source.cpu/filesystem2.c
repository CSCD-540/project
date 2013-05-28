#include "filesystem2.h"

/* global variables */
int fs_idCounter;
int filesystem[FS_SIZE];
List* iNodes;


void fs_initialize() {
  int i;  
  
  fs_idCounter = 0;
  iNodes = list_create("iNodes");
  
  for (i = 0; i < FS_SIZE; i++)
    filesystem[i] = FS_NULL;
  
  if (FS_DEBUG) {
    heavyLine();
    printf("fs_initialize()\n");
    heavyLine();
    fs_dump();
    lightLine();
  }
  
}

void fs_close() {
  list_destroy(iNodes, inode_destroy);
}

void fs_dump() {
  
  printf("  fs_idCounter:  %4d \n", fs_idCounter);
  printf("  iNodes->count: %4d \n", iNodes->count);

  list_print(iNodes, inode_print);
  
  if (FS_VERBOSE)
    fs_dumpAllData();
    
}


void fs_dumpData(int size, int* data) {
  int i;
  
  printf("    ");
  for (i = 0; i < 20; i++)
    printf("%4d", i);
  printf("\n");

  for (i = 0; i < size; i++) {
    // row number
    if (i % 20 == 0) 
      printf("%4d", i / 20);
      
    printf("%4d", data[i]);
    
    // row line break
    if (i % 20 == 19)
      printf("\n");
  }
    
  printf("\n");
}


void fs_dumpAllData() {
  int i;
  int j;
  
  printf("\n  filesystem[]:\n");
  fs_dumpData(FS_SIZE, filesystem);
}


int fs_dataIsValid(int size, int* data) {
  int i;
  
  for (i = 0; i < size; i++)
    if (data[i] == FS_NULL)
      return FALSE;
  
  return TRUE;
}


int fs_import(char* filename) {
  int i;
  int j;
  
  int processes;
  int process;
  int size;
  
  char* s;
  
  FILE* fp;
  
  fp = fopen(filename, "r");
  
  free((char*)getString(fp));  
  processes = getInt(fp);
  
  printf("reading %d processes from: %s \n", processes, filename);
  lightLine();
  
  for (i = 0; i < processes; i++) {
    free((char*)getString(fp));
    process = getInt(fp);
    
    free((char*)getString(fp));
    
    size = getInt(fp);
    
    int data[size];
    
    for (j = 0; j <= size; j++)
      data[j] = getInt(fp);
  
    printf("process: %4d \t size: %4d \n", i, size);
    for (j = 0; j < size; j++) 
      printf("%5d", data[j]);
    
    printf("\n");
      
    fs_addFile(FS_DEFAULT_FILENAME, size, data);
    lightLine();
  }
    
  fclose(fp);
  
  return processes;
}


/* returns id */
int fs_addFile(char* name, int size, int* data) {  
  int id;
  int start;
  
  if (FS_DEBUG) {
    heavyLine();
    printf("fs_addFile(\"%s\", %d, ...)\n", name, size);
    if (FS_VERBOSE)
      fs_dumpData(size, data);
    heavyLine();
  }
  
  if (!fs_dataIsValid(size, data))
    return -1;
    
  start = fs_addData(size, data);
  id = fs_addINode(name, start, size);
  
  if (FS_DEBUG)
    fs_dump();
    
  return id;
}


/* returns start position */
int fs_addData(int size, int* data) {
  int i;
  int start;
  int count;
  
  if (FS_DEBUG) {
    lightLine();
    printf("fs_addData(%d, ...)\n", size);
    if (FS_VERBOSE)
      fs_dumpData(size, data);
    lightLine();
  }
  
  i = 0;
  count = 0;
  
  do {
    if (filesystem[i] == FS_NULL)
      count++;
    else
      count = 0;
    
    if (count >= size)
      break;
  } while (i++ < FS_SIZE);
  
  
  /* not enough room */
  if (i > FS_SIZE)
    return -1;
     
  start = i - size + 1;
  
  for (i = 0; i < size; i++) 
    filesystem[start + i] = data[i];
    
  return start;
}

/* returns id */
int fs_addINode(char* name, int start, int size) {
  INode* node;
  
  if (FS_DEBUG) {
    lightLine();
    printf("fs_addINode(\"%s\", %d, %d)\n", name, start, size);
    lightLine();
  }
  
  /* outside of bounds */
  if (start < 0)
    return -1;
  
  node = inode_create(++fs_idCounter, name, start, size);
  list_addLast(iNodes, node);
  
  return fs_idCounter;
}


void fs_removeFile(int id) {
  int i;
  INode* node;
  
  if (FS_DEBUG) {
    lightLine();
    printf("removeFile(%d) \n", id);
    lightLine();
  }
  
  node = list_popNode(iNodes, (void*)id, inode_compareById);
  
  if (node == NULL)
    return;
  
  for (i = 0; i < node->size; i++)
    filesystem[node->start + i] = -1;
  
  free(node);
}

void fs_removeAllFiles() {
  int  i;
  int  count;
  INode* node;
  
  if (FS_DEBUG) {
    lightLine();
    printf("removeAllFiles()\n");
    lightLine();
  }
  
  count = iNodes->count;
  for (i = 0; i < count; i++) {
    node = list_peekFirst(iNodes);
    inode_print(node);
    fs_removeFile(node->id);
  }
}

INode* fs_getNode(int id) {
  return (INode*)list_peekNode(iNodes, (void*)id, inode_compareById);;
}

int* fs_getData(int id) {
  int i;
  int* data;
  INode* node;
  
  node = list_peekNode(iNodes, (void*)id, inode_compareById);
  data = calloc(node->size, sizeof(int));
  
  for (i = 0; i < node->size; i++)
    data[i] = filesystem[node->start + i];
  
  return data;
}

int* fs_getPage(int id, int start, int size) {
  int i;
  int* data;
  INode* node;
  
  node = list_peekNode(iNodes, (void*)id, inode_compareById);
  data = calloc(size, sizeof(int));
  
  for (i = 0; i < size; i++) {
    if (node->start + start + i < node->start + node->size)
      data[i] = filesystem[node->start + start + i];
    else
      data[i] = FS_NULL;
  }
  
  return data;
}


int fs_getINodeCount() {
  return iNodes->count;
}

void fs_ls() {
  if (FS_DEBUG) {
    lightLine();
    printf("fs_ls()\n");
    lightLine();
  }
  
  list_print(iNodes, inode_print);
  if (FS_VERBOSE)
    fs_dumpAllData();
  
  if (FS_DEBUG)
    lightLine();
}

void fs_copy(char* name, char* newName) {
  int* data;
  INode* node;
  
  if (FS_DEBUG) {
    lightLine();
    printf("fs_copy(%s, %s)\n", name, newName);
    lightLine();
  }
  
  node = list_peekNode(iNodes, (void*)name, inode_compareByName);
  data = fs_getData(node->id);
 
  fs_addFile(newName, node->size, data);
  free(data);
}