#include "pagetable.h"


int pt[MAXPRO][MAXMEM / PAGE_SIZE];     // inverted page table
int ptCount[MAXPRO][MAXMEM / PAGE_SIZE];  // used to track the last time a page was used
int ptCounter = 0;                // LRU counter

int pages = MAXMEM / PAGE_SIZE;

void pt_initialize() {
  int i;
  int j;
  
  if (PT_DEBUG) {
    heavyLine();
    printf("pt_initialize()\n");
    heavyLine();
  }
  
  for (i = 0; i < MAXPRO; i++)
    for (j = 0; j < pages; j++)
      pt[i][j] = -1;

}

void pt_dump() {
  int i;
  int j;
  
  if (PT_DEBUG) {
    lightLine();
    printf("pt_dump()\n");
    lightLine();
  }
  
  for (i = 0; i < MAXPRO; i++) {
    printf("process: %d \n", i);
    for (j = 0; j < pages; j++) {
      printf("%5d", pt[i][j]);
    }
    printf("\n\n");
  }
}

int pt_getPageNumber(int instruction) {
  return instruction / PAGE_SIZE;
}

int pt_getPageOffset(int instruction) {
  return instruction % PAGE_SIZE;
}

int pt_getPage(int vPage) {
  int i;
  
  for (i = 0; i < pages; i++)
    if (pt[i] == vPage)
      return i;
  
  return PT_MISS;
}

int pt_requestInstruction(int process, int instruction) {
  int vpage;
  int pPage;
  int offset;
  int address;
  
  vpage  = pt_getPageNumber(instruction);  
  offset = pt_getPageOffset(instruction);
  pPage  = pt_getPage(page);

  if (pPage == PT_MISS) {
    pt_evictPage(process);
    pPage = pt_loadPage(process, page);
  }
  
  address = (pPage * PAGE_SIZE) + offset;
  
  return mem[process][address];

}


/*

void pt_loadPage(int process, int page) {
  int virtualPage;
  
  virtualPage = pt_EvictPage(process);
  pt[process][virtualPage] = page;
  ptLRU[process][virtualPage] = ptCounter++;
}

// -1 indicates page fault
int pt_getVirtualPage(int process, int page) {
  int virtualPage = -1;
  int i;
  
  for (i = 0; i < PT_VIRTUAL_PAGES; i++) 
    if (pt[process][i] == page)
      virtualPage = i;
  
  return i;
  
}

int pt_evictPage(int process) {
  int i;
  int virtualPage = -1;
  int min = ptCounter;
  
  // Check for empty page
  for (i = 0; i < PT_VIRTUAL_PAGES; i++)
    if (pt[process][i] == -1)
      return i;
  
  // Check for least recently used page
  for (i = 0; i < PT_VIRTUAL_PAGES; i++)
    if (ptLRU[process][i] < min) {
      min = ptLRU[process][i];
      virtualPage = i;
    }
  
  return virtualPage;
}
*/