/* includes */
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "inode.h"

/* #defines */
#define FS_DEBUG         0
#define FS_VERBOSE       0  

#define FS_NULL         -1
#define FS_DEFAULT_FILENAME   "unknown"

#define FS_SIZE        100
#define FS_NAME_SIZE    16
#define FS_INODES       10

#define FS_COLUMNS      20

#define PAGE_SIZE        4

#define true 1;
#define false 0;

/* prototypes */
void fs_initialize();
void fs_close();

void fs_dump();
void fs_dumpData(int size, int* data);
void fs_dumpAllData();
int fs_dataIsValid(int size, int* data);

int fs_import(char* filename);

int fs_addFile(int process, char* name, int size, int* data);
int fs_addData(int size, int* data);
int fs_addINode(int process, char* name, int start, int size);

void   fs_removeFile(int id);
void   fs_removeAllFiles();

INode* fs_getNode(int id);
int*   fs_getData(int id);
int*   fs_getPage(int id, int start, int size);

int    fs_getINodeCount();

void   fs_ls();
void   fs_copy(char* name, char* newName);