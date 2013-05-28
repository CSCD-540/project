#include "filesystem2.h"
#include "helper.h"

#define TEST_FILES        4
#define TEST_DATA_SIZE   30

/* filesystem2.c tester */

int main() {
  int i;
  int j;
  
  heavyLine();
  printf("filesystem self test\n");
  heavyLine();
  printf("\n");

  fs_initialize();
  
  int testData[TEST_DATA_SIZE];

  // creating test data
  for (i = 0; i < TEST_DATA_SIZE; i++)
    testData[i] = 100 + i;
  
  // add test data
  for (i = 0; i < TEST_FILES; i++) {
    fs_addFile("test file", TEST_DATA_SIZE, testData);
  }
  
  // list the files
  fs_ls();
  
  // remove file with id of 2
  fs_removeFile(2);

  printf("\n");

  // list the files
  fs_ls();
  
  // invalid data
  int invalidData[3] = {FS_NULL, FS_NULL, FS_NULL};
  
  // add invalid data
  fs_addFile("invalid data", 3, invalidData);

  // list the files
  fs_ls();

  fs_addFile("test data", TEST_DATA_SIZE, testData);
  
  // list the files
  fs_ls();
  
  // remove all files
  fs_removeAllFiles();

  // list the files
  fs_ls();
  
  fs_import("./programs.cpu/prog1out.cpu");
  
  fs_ls();
  
  fs_copy("fname", "fname2");
  
  fs_ls();

  int pages;
  int* d;
  INode* node;
  
  node = (INode*)fs_getNode(7);
  inode_print(node);
  
  pages = node->size / PAGE_SIZE;
  if (node->size % PAGE_SIZE != 0)
    pages++;
  
  for (i = 0; i < pages; i++) {
    d = (int*)fs_getPage(7, i * PAGE_SIZE, PAGE_SIZE);
    printf("page: %d \n", i);
    for (j = 0; j < 4; j++)
      printf("\tdata: %d ", d[j]);
    printf("\n");
    free(d);
  }
  
  fs_close();

   return 0; 
}