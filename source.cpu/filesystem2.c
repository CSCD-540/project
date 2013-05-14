/* temporary filesystem */

/* includes */
#include <stdlib.h>
#include <string.h>

/* #defines */
#define FS_SIZE        100
#define FS_NAME_SIZE    16
#define FS_INODES       10

#define FS_COLUMNS      20

#define true 1;
#define false 0;

/* structs */
struct inode {
  int  id;
  char name[FS_NAME_SIZE];
  int  used;
  int  start;
  int  size;
};

typedef struct inode INode;

/* global variables */
int fileCount = 0;

int filesystem[FS_SIZE];
// TODO: convert to linked list
struct inode inodes[FS_INODES];


/* prototypes */
void fs_test();
void fs_initialize();
int  fs_import(char* filename);

void fs_dump();
void fs_dumpData(int id);
void fs_dumpAllData();
void fs_dumpINode(int id);
void fs_dumpINodeByIndex(int index);
void fs_dumpAllINodes();

int  fs_getINodeCount();
int  fs_getINodeIndex(int id);

// TODO: Fix these
void fs_getINode(int id, INode* node);
void fs_getAllINodes(struct inode* nodes);

int  fs_getFileId(char name[FS_NAME_SIZE]);

void fs_getFile(int id, int offset, int size, int* page);
void fs_getFileByName(char name[FS_NAME_SIZE], int offset, int size, int* page);

// TODO: add file names to program file
//int  fs_addFile(char name[FS_NAME_SIZE], int size, int* data);
int  fs_addFile(int size, int* data);
//int  fs_addINode(char name[FS_NAME_SIZE], int start, int size);
int  fs_addINode(int start, int size);
int  fs_addData(int size, int* data);

void fs_removeFile(int id);
void fs_defrag();

int  fs_nameExists(char name[FS_NAME_SIZE]);


/* helper functions */
void heavyLine() {
  printf("==============================\n");
}

void lightLine() {
  printf("------------------------------\n");
}

void starLine() {
  printf("******************************\n");
}


/* filesystem functions */
void fs_test() {
  int i;
  int j;
  
  heavyLine();
  printf("filesystem self test\n");
  heavyLine();
  printf("\n");
  lightLine();

  printf("fs_initialize()\n");
  fs_initialize();
  lightLine();
  
  printf("inodes: \n");
  fs_dumpAllINodes();
  lightLine();
  
  printf("filesystem: \n");
  fs_dumpAllData();
  lightLine();
  
  printf("fs_getINodeCount() = %d\n", fs_getINodeCount());
  lightLine();
  
  int testFiles = 4;
  int testDataSize = 30;
  int testData[testDataSize];
  
  printf("fs_addFile()\n");
  for (i = 0; i < testDataSize; i++)
    testData[i] = 100 + i;
  
  for (i = 0; i < testFiles; i++) {
    printf("adding file %4d \n", i);
    fs_addFile(testDataSize, testData);
  }
    
  lightLine();
  
  printf("fs_getINodeCount() = %d\n", fs_getINodeCount());
  lightLine();
  
  fs_dump();
  lightLine();
  
  printf("filesystem: \n");
  fs_dumpAllData();
  lightLine();
  
  printf("fs_removeFile() \n");
  lightLine();
  
  for (i = 0; i < testFiles; i++) {
    printf("removing file %4d \n", i);
    fs_removeFile(i);
    fs_dumpAllData();
    lightLine();
  }
  
  for (i = 0; i < testFiles; i++) {
    printf("adding file %4d \n", i);
    fs_addFile(testDataSize, testData);
  }
  
  lightLine();
  
  fs_dumpAllData();
  lightLine();
  
  printf("removing file %4d \n", 1);
  fs_removeFile(1);
  lightLine();
  
  fs_dumpAllData();
  lightLine();
  
  printf("adding file \n");
  fs_addFile(testDataSize, testData);
  lightLine();
  
  fs_dumpAllData();
  lightLine();
  
   fs_dump();
   lightLine();
  
  
}


void fs_initialize() {
  int i;
  
  for (i = 0; i < FS_SIZE; i++)
    filesystem[i] = -1;
  
  for (i = 0; i < FS_INODES; i++)
    inodes[i].used = false;
}

/* returns number of processes */
int fs_import(char* filename) {
  int i;
  int j;
  
  int processes;
  int process;
  int size;
  
  FILE* fp;
  
  
  fp = fopen(filename, "r");
  
  getString(fp);
  processes = getInt(fp);
  
  printf("reading %d processes from: %s \n", processes, filename);
  lightLine();
  
  for (i = 0; i < processes; i++) {
    getString(fp);
    process = getInt(fp);
    
    getString(fp);
    size = getInt(fp);
    
    int data[size];
    
    for (j = 0; j <= size; j++)
      data[j] = getInt(fp);
  
    printf("process: %4d \t size: %4d \n", i, size);
    for (j = 0; j < size; j++) {
      printf("%5d", data[j]);
    }
    printf("\n");
      
    fs_addFile(size, data);
    lightLine();
  }
    
  fclose(fp);
  
  return processes;
}

void fs_dump() {
  int i;
  
  for(i = 0; i < FS_INODES; i++) 
    if (inodes[i].used) {
      fs_dumpINodeByIndex(i);
      lightLine();
      fs_dumpData(i);
      lightLine();
    }
}

void fs_dumpData(int id) {
  int i;
  struct inode node;

  node = inodes[id];
  
  for(i = 0; i < node.size; i++) 
    printf("%5d", filesystem[node.start + i]);
  
  printf("\n");
}

void fs_dumpAllData() {
  int i;
  int j;
  
  printf("    ");
  for (i = 0; i < 20; i++)
    printf("%4d", i);
  printf("\n");

  for (i = 0; i < FS_SIZE; i++) {
    if (i % 20 == 0) 
      printf("%4d", i / 20);
      
    printf("%4d", filesystem[i]);
    if (i % 20 == 19)
      printf("\n");
  }

  
  printf("\n");
}


void fs_dumpINode(int id) {
  struct inode node;
  
  //node = inodes[id];
  fs_getINode(id, &node);
  
  printf("  ");
  printf("id:    %4d  \t", node.id);
  printf("size:  %4d  \t", node.size);
  printf("start: %4d  \t", node.start);
  printf("used:  %4d  \t", node.used);
  printf("name:  %16s \n", node.name);
}

void fs_dumpINodeByIndex(int index) {
  struct inode node;
  
  node = inodes[index];
    
  printf("  ");
  printf("id:    %4d  \t", node.id);
  printf("size:  %4d  \t", node.size);
  printf("start: %4d  \t", node.start);
  printf("used:  %4d  \t", node.used);
  printf("name:  %16s \n", node.name);
}

void fs_dumpAllINodes() {
  int i;
  
  for (i = 0; i < FS_INODES; i++) {
    fs_dumpINode(i);
  }
}

int fs_getINodeCount() {
  int i;
  int count;
  
  count = 0;
  
  for(i = 0; i < FS_INODES; i++) 
    if (inodes[i].used) 
      count++;
  
  return count;
}

/* return the inodes index */
int fs_getINodeIndex(int id) {
  int i;
  int index;
  
  for (i = 0; i < FS_INODES; i++) {
    if (inodes[i].used && inodes[i].id == id)
      index = i;
  }

  return index;
}

void fs_getINode(int id, INode* node){
  int index;
  
  index = fs_getINodeIndex(id);
  printf("index: %d", index);
  node = &inodes[index];
  
}


void fs_getAllINodes(struct inode* nodes) {
  int i;
  int index;
  
  index = 0;
  
  nodes = calloc(fs_getINodeCount(), sizeof(struct inode));
  
  for (i = 0; i < FS_INODES; i++)
    if (inodes[i].used)
      nodes[index++] = inodes[i];
}

int  fs_getFileId(char name[FS_NAME_SIZE]) {
  int i;
  
  for (i = 0; i < FS_INODES; i++)
    if (inodes[i].used)
      if (strcmp(inodes[i].name, name) == 0)
        return i;
  
  return -1;
}

void fs_getFile(int id, int offset, int size, int* page) {
  int  i;
  int  index;
  struct inode node;
  
  //fs_getINode(id, node);
  index = fs_getINodeIndex(id);
  
  node = inodes[index];  

  for (i = 0; i < size; i++) {
    index = node.start + offset + i;
    page[i] = filesystem[index];
  }
  
}


void fs_getFileByName(char name[FS_NAME_SIZE], int offset, int size, int* page) {
  int id;
  
  id = fs_getFileId(name);
  
  return fs_getFile(id, offset, size, page);
  
}

/* return id */
//int fs_addFile(char name[FS_NAME_SIZE], int size, int* data) {
int fs_addFile(int size, int* data) {
  int i;
  int id;
  int start;
    
  start = fs_addData(size, data);
  //id = fs_addINode(name, start, size);
  id = fs_addINode(start, size);
}

/* returns id */
//int fs_addINode(char name[FS_NAME_SIZE], int start, int size) {
int fs_addINode(int start, int size) {
  int i;
  
  /* outside of bounds */
  if (start < 0)
    return -1;
  
  for (i = 0; i < FS_INODES; i++)
    if (!inodes[i].used)
      break;

  //strcpy(name, inodes[i].name);
  inodes[i].id    = fileCount++;
  inodes[i].used  = true;
  inodes[i].start = start;
  inodes[i].size  = size;
}

/* returns start position */
int fs_addData(int size, int data[]) {
  int i;
  int start;
  int count;
  
  i = 0;
  count = 0;
  
  do {
    if (filesystem[i] == -1)
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


void fs_removeFile(int id) {
  int i;

  for (i = 0; i < inodes[id].size; i++)
    filesystem[inodes[id].start + i];
  
  inodes[id].used = false;
  
  for (i = 0; i < inodes[id].size; i++)
    filesystem[inodes[id].start + i] = -1;
}


// TODO: defrag
void fs_defrag() {
  
}


int fs_nameExists(char name[FS_NAME_SIZE]) {
  int i;
  
  for (i = 0; i < FS_INODES; i++)
    if (strcmp(inodes[i].name, name) == 0)
      return true;
  
  return false;
}