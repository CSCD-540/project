#include "pagetable.h"
#include "filesystem2.h"


extern int mem[MAXPRO][MAXMEM];




int main() {
  int i;
  int j;
  
  for (i = 0; i < MAXPRO; i++)
    for (j = 0; j < MAXMEM; j++)
      mem[i][j] = -1;
  
  for (i = 0; i < FS_SIZE; i++)
    fs[i] = -1;
  
  pt_initialize();  
  pt_dump();
  
  for (i = 0; i < 20; i++)
    printf("instruction: %4d  page: %4d  offset: %4d \n", i, pt_getPageNumber(i), pt_getPageOffset(i));
  
  return 0;
}