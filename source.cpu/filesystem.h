#ifndef FILESYSTEM2_H_INCLUDED
  #define FILESYSTEM2_H_INCLUDED
  
  /* includes */
  #include <stdlib.h>
  #include <stdio.h>
  #include <string.h>

  #include "config.h"
  #include "inode.h"

  /**
 * offset constants
 * All are defined as positive values, so node offsets should be subtracted
 * FS_PREV_NODE_OFFSET or FS_PREV_REC_OFFSET are not used, but they are included in case they are needed in the future
 */
#define FS_PREV_REC_OFFSET 0
#define FS_NEXT_REC_OFFSET 1
#define FS_FID_REC_OFFSET 2
#define FS_SIZE_REC_OFFSET 3
#define FS_PROC_COUNT_REC_OFFSET 4
#define FS_PROC_VALUES_REC_OFFSET 5
#define FS_PROC_SIZE 6
#define FS_DATA_REC_OFFSET (FS_PROC_VALUES_REC_OFFSET+FS_PROC_SIZE)
#define FS_PREV_NODE_OFFSET 0
#define FS_NEXT_NODE_OFFSET 1
#define FS_NAME_NODE_OFFSET 2
#define FS_NAME_NODE_SIZE 16
#define FS_DATA_NODE_OFFSET (FS_NAME_NODE_OFFSET+FS_NAME_NODE_SIZE)
#define FS_LOCATION_NODE_OFFSET (1+FS_NAME_NODE_OFFSET+FS_NAME_NODE_SIZE)

#define FILESYSTEM_SIZE 4096
#define DEFRAG 0.6
#define FS_DEBUG 0


  /* global variables */
  int fs_idCounter;//no idea, might not be needed
  int filesystem[FILESYSTEM_SIZE];
  int lastFree;//tracks last free position node
  int lastFill;//tracks last filled position node
  int lastFid;//tracks last file pointer assigned
  int end;//tracks end of meta data
  int freeSpace;//tracks space not used, this will be useful for determining when to defragment
  List* iNodes;

/**
 * prototypes
 */
void fs_init();
void fs_store(int len, void* data, char* name, int numOfProc, int* procSizes);
void fs_nonFragment(int len, void* data, char* name, int numOfProc, int* procSizes);
void fs_fragment();//not currently used
void fs_addFree(int index);
void fs_addFill(int index);

int fs_defragCheck();
void fs_defrag();

int fs_findRecSize(int index);
int fs_findFile(int fid);

void fs_getRecData(int index, int* buf, int size);
void fs_getRecMetaData(int index, int* buf, int size);
void fs_get(int fid, int offset, int length, char* toReturn);

void fs_dump();
void fs_list();

//interface stuff
void fs_initialize();
void fs_close();//not currently used

void fs_import(char* path, char* name);
void fs_copy(int fid, char* name);
int fs_addFile(char* name, int fileSize, int processes, int* processStart, int* processSize, int* data);

void fs_removeFile(int fid);
void fs_removeAll();

int fs_getData(int id, char* buf);
int fs_getPage(int id, int process, int start, int size, char* buf);

int fs_getINodeCount();
INode* fs_getNode(int id);
void fs_getAllNodes();

int fs_getProcessCount(int id);
int fs_getProcessSize(int id, int process);

void fs_nodeList();

//  void fs_dumpAllData();
//  int fs_dataIsValid(int size, int* data);


//  int fs_addData(int size, int* data);
//  int fs_addINode(char* name, int fileStart, int fileSize, int processes, int* processStart, int* processSize);


//  void fs_getAllNodes(INode *nodes[]);

#endif