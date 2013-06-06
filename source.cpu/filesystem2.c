#include "filesystem2.h"


/* global variables */
int fs_idCounter;
int filesystem[FS_SIZE];
List* iNodes;


void fs_initialize() {
  int i;  
  
  if (FS_DEBUG) {
    heavyLine();
    printf("fs_initialize()\n");
  }
  
  fs_idCounter = 0;
  iNodes = list_create("iNodes");
  
  for (i = 0; i < FS_SIZE; i++)
    filesystem[i] = FS_NULL;
  
  if (FS_VERBOSE) 
    fs_dump();
    
  if (FS_DEBUG)
    heavyLine();
  
  
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

void fs_dumpAllData() {
  int i;
  int j;
  
  printf("\n  filesystem[]:\n");
  dumpData(FS_SIZE, filesystem);
}


int fs_dataIsValid(int size, int* data) {
  int i;
  
  for (i = 0; i < size; i++)
    if (data[i] == FS_NULL)
      return FALSE;
  
  return TRUE;
}


int fs_import(char* filename, char* name) {
  int i;
  int j;
  
  int fileSize;
  int processes;
  int processStart[MAXPRO];
  int processSize[MAXPRO];
  
  FILE* fp;
  
  if (FS_DEBUG) {
    heavyLine();
    printf("fs_import(%s, %s)\n", filename, name);
    heavyLine();
  }
  
  fp = fopen(filename, "r");
  
  // discard string: processes
  free((char*)getString(fp));
  processes = getInt(fp);
  
  // discard string: size
  free((char*)getString(fp));
  fileSize = 0;
  for (i = 0; i < processes; i++) {
    processStart[i] = fileSize;
    processSize[i] = getInt(fp) + 1;
    fileSize += processSize[i];
  }
  
  int data[fileSize];
  for (i = 0; i < fileSize; i++)
    data[i] = getInt(fp);
  
  if (FS_VERBOSE)
    dumpData(fileSize, data);
  
  fs_addFile(name, fileSize, processes, processStart, processSize, data);
  
  
  fclose(fp);  
}


/* returns id */
int fs_addFile(char* name, int fileSize, int processes, int* processStart, int* processSize, int* data) {  
  int i;
  int id;
  int fileStart;
  
  if (FS_DEBUG) {
    heavyLine();
    printf("fs_addFile(\"%s\", %d, ..., ..., ...)\n", name, fileSize);

    if (FS_VERBOSE) {
      printf("processStart: ");
      for (i = 0; i < MAXPRO; i++)
        printf("%d ", processStart[i]);
      printf("\nprocessSize: ");
      for (i = 0; i < MAXPRO; i++)
        printf("%d ", processSize[i]);
      dumpData(fileSize, data);
    }
    heavyLine();
  }
  
  if (!fs_dataIsValid(fileSize, data))
    return -1;
    
  fileStart = fs_addData(fileSize, data);
  id = fs_addINode(name, fileStart, fileSize, processes, processStart, processSize);

  if (FS_VERBOSE)
    fs_dump();
    
  return id;
}


/* returns start position */
int fs_addData(int fileSize, int* data) {
  int i;
  int fileStart;
  int count;
  
  if (FS_DEBUG) {
    lightLine();
    printf("fs_addData(%d, ...)\n", fileSize);
    if (FS_VERBOSE)
      dumpData(fileSize, data);
    lightLine();
  }
  
  i = 0;
  count = 0;
  
  while (i < FS_SIZE && count < fileSize) {
    if (filesystem[i] == FS_NULL)
      count++;
    else
      count = 0;
    
    i++;
  }
  
  /* not enough room */
  if (count < fileSize) {
    printf("insufficient disk space\n");
    return -1;
  }
     
  fileStart = i - fileSize;
  
  for (i = 0; i < fileSize; i++) 
    filesystem[fileStart + i] = data[i];
    
  return fileStart;
}

/* returns id */
int fs_addINode(char* name, int fileStart, int fileSize, int processes, int* processStart, int* processSize) {
  int i;
  INode* node;
  
  if (FS_DEBUG) {
    lightLine();
    printf("fs_addINode(\"%s\", %d, %d, ..., ...)\n", name, fileStart, fileSize);
    
    if (FS_VERBOSE) {
      printf("processStart: [");
      for (i = 0; i < processes; i++)
        printf("%d ", processStart[i]);
      printf("]\nprocessSize: [");
      for (i = 0; i < processes; i++)
        printf("%d ", processSize[i]);
      printf("]\n");
    }
    lightLine();
  }
  
  /* outside of bounds */
  if (fileStart < 0) {
    printf("start position is outside of bounds\n");
    return -1;
  }
  
  node = inode_create(++fs_idCounter, name, fileStart, fileSize, processes, processStart, processSize);
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
  
  for (i = 0; i < node->fileSize; i++)
    filesystem[node->fileStart + i] = -1;
  
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

void fs_getAllNodes(INode *nodes[]) {
  int i;
  
  i = 0;
  LIST_FOREACH(iNodes, first, next, cur) {
    nodes[i++] = (INode*)cur->value;
  }
}

int* fs_getData(int id) {
  int i;
  int* data;
  INode* node;
  
  node = list_peekNode(iNodes, (void*)id, inode_compareById);
  data = calloc(node->fileSize, sizeof(int));
  
  for (i = 0; i < node->fileSize; i++)
    data[i] = filesystem[node->fileStart + i];
  
  return data;
}


int* fs_getPage(int id, int process, int offset, int size) {
  int i;
  int position;
  int* data;
  INode* node;
  
  if (FS_DEBUG) {
    lightLine();
    printf("fs_getPage(%d, %d, %d, %d)\n", id, process, offset, size);
    lightLine();
  }
  
  node = list_peekNode(iNodes, (void*)id, inode_compareById);
  data = calloc(size, sizeof(int));
  
  for (i = 0; i < size; i++) {
    position = node->fileStart + node->processStart[process] + offset + i;
    if (position < node->fileStart + node->fileSize)
      data[i] = filesystem[position];
    else
      data[i] = FS_NULL;
  }
  
  return data;
}


int fs_getINodeCount() {
  return iNodes->count;
}

int fs_getProcessCount(int id) {
  INode* node;
  
  if (FS_VERBOSE) {
    lightLine();
    printf("fs_getPage(%d)\n", id);
    lightLine();
  }
  
  node = list_peekNode(iNodes, (void*)id, inode_compareById);
  return node->processes;
}


int fs_getProcessSize(int id, int process) {
  INode* node;
  
  if (FS_VERBOSE) {
    lightLine();
    printf("fs_getPage(%d, %d)\n", id, process);
    lightLine();
  }
  
  node = list_peekNode(iNodes, (void*)id, inode_compareById);
  
  return node->processSize[process];
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

  fs_addFile(newName, node->fileSize, node->processes, node->processStart, node->processSize, data);
  free(data);
}