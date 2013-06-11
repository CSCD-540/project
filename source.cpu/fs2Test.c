#include "config.h"
#include "filesystem2.h"


#define TEST_FILES        4
#define TEST_DATA_SIZE   30

/* filesystem2.c tester */

int main() {
  int i;
  int j;
  int k;
  
  heavyLine();
  printf("filesystem self test\n");
  heavyLine();
  printf("\n");

  fs_initialize();
  
  // import programs
  fs_import("./programs.cpu/prog1out.cpu", "prog1");
  fs_import("./programs.cpu/prog2out.cpu", "prog2");
  
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
  int pStart[1] = {0};
  int pSize[1] = {3};
  fs_addFile("invalid data", 3, 1, pStart, pSize, invalidData);

  // list the files
  fs_ls();
  fs_import("./programs.cpu/prog3out.cpu", "prog2");
  
  // list the files
  fs_ls();
  
  // remove all files
  fs_removeAllFiles();

  // list the files
  fs_ls();

  fs_import("./programs.cpu/prog1out.cpu", "prog1");
  fs_import("./programs.cpu/prog2out.cpu", "prog2");
  fs_import("./programs.cpu/prog3out.cpu", "prog3");
  fs_import("./programs.cpu/prog4out.cpu", "prog4");

  
  fs_ls();
  fs_removeFile(6);
  fs_dumpAllData();

  fs_ls();  
  fs_copy(5, "progA");
  
  fs_ls();

  // testing paging

  INode* node;
  node = (INode*)fs_getNode(4);

  int* page;

  page = calloc(PAGE_SIZE, sizeof(int));

  for (i = 0; i < node->processes; i++) {
    heavyLine();
    printf("process: %d \n", i);
    for (j = 0; j < (node->processSize[i] / PAGE_SIZE); j++) {
      page = fs_getPage(4, i, j * PAGE_SIZE, PAGE_SIZE);
      for (k = 0; k < PAGE_SIZE; k++)
        printf("%4d ", page[k]);
      printf("\n");
    }
  }
  heavyLine();
  


  
  fs_close();

   return 0; 
}