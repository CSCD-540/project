#ifndef FILESYSTEM2_H_INCLUDED
  #define FILESYSTEM2_H_INCLUDED
  
  /* includes */
  #include <stdlib.h>
  #include <stdio.h>
  #include <string.h>

  #include "config.h"
  #include "inode.h"

  /* global variables */
  int fs_idCounter;
  int filesystem[FS_SIZE];
  List* iNodes;


  void fs_initialize();
  void fs_close() ;

  void fs_dump();
  void fs_dumpAllData();

  int fs_dataIsValid(int size, int* data);

  int fs_import(char* filename, char* name);

  int fs_addFile(char* name, int fileSize, int processes, int* processStart, int* processSize, int* data);
  int fs_addData(int size, int* data);
  int fs_addINode(char* name, int fileStart, int fileSize, int processes, int* processStart, int* processSize);

  void fs_removeFile(int id);
  void fs_removeAllFiles();

  INode* fs_getNode(int id);
  void fs_getAllNodes(INode *nodes[]);
  int* fs_getData(int id);
  int* fs_getPage(int id, int process, int start, int size);

  int fs_getINodeCount();
  int fs_getProcessCount(int id);
  int fs_getProcessSize(int id, int process);

  void fs_ls();
  void fs_copy(char* name, char* newName);

#endif