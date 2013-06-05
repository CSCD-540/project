#include "pagetable.h"

extern int mem[MAXPRO][MAXMEM];

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

int pt_getPage(int process, int vPage) {
  int i;
  
  for (i = 0; i < pages; i++)
    if (pt[process][i] == vPage)
      return i;
  
  return PT_MISS;
}

int pt_requestInstruction(int process, int vAddress) {
  int vPage;
  int pPage;
  int offset;
  int pAddress;
  
  vPage  = pt_getPageNumber(vAddress);  
  offset = pt_getPageOffset(vAddress);
  pPage  = pt_getPage(process, vPage);

  if (pPage == PT_MISS)
    pPage = pt_loadPage(process, vPage);
  
  pAddress = (pPage * PAGE_SIZE) + offset;
  
  return mem[process][pAddress];
}


int pt_evictPage(int process) {
  /*
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
  */ 
  return -1;
}


int pt_loadPage(int process, int page) {
  /*
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
  */
  return -1;
}


