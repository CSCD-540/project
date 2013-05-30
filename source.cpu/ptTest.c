

#include "pagetable.h"

int main() {
  int i;
  
  pt_initialize();  
  pt_dump();
  
  for (i = 0; i < 20; i++)
    printf("instruction: %4d  page: %4d  offset: %4d \n", i, pt_getPageNumber(i), pt_getPageOffset(i));
  
  return 0;
}