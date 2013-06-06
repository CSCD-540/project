#include "pagetable.h"

extern int mem[MAXPRO][MAXMEM];
extern currentProgramId;

int pt[MAXPRO][MAXMEM / PAGE_SIZE];       // inverted page table
int ptCount[MAXPRO][MAXMEM / PAGE_SIZE];  // used to track the last time a page was used
int ptCounter = 0;                        // LRU counter

int ptSize = MAXMEM / PAGE_SIZE;

void pt_initialize() {
  int i;
  int j;
  
  if (PT_DEBUG) {
    heavyLine();
    printf("pt_initialize()\n");
  }
  
  for (i = 0; i < MAXPRO; i++)
    for (j = 0; j < ptSize; j++)
      pt[i][j] = PT_NULL;

  if (PT_VERBOSE) 
    pt_dump();

  if (PT_DEBUG) 
    heavyLine();

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
    dumpData(ptSize, pt[i]);
    printf("\n");
  }
  
}

int pt_getPageNumber(int instruction) {
  if (PT_VERBOSE) {
    lightLine();
    printf("pt_getPageNumber(%d)\n", instruction);
    lightLine();
  }
  
  return instruction / PAGE_SIZE;
}


int pt_getPageOffset(int instruction) {
  if (PT_VERBOSE) {
    lightLine();
    printf("pt_getPageOffset(%d)\n", instruction);
    lightLine();
  }
  
  return instruction % PAGE_SIZE;
}

int pt_getPage(int process, int virtualPage) {
  int i;

  if (PT_VERBOSE) {
    lightLine();
    printf("pt_getPage(%d, %d)\n", process, virtualPage);
    lightLine();
  }
  
  
  for (i = 0; i < ptSize; i++)
    if (pt[process][i] == virtualPage)
      return i;

  heavyLine();
  printf("PT_MISS!\n");
  heavyLine();
  
  return PT_MISS;
}

// return page table id
int pt_evictPage(int process) {
  int i;
  int pageTableId;
  int min = ptCounter;
  
  if (PT_DEBUG) {
    lightLine();
    printf("pt_evictPage(%d)\n", process);
    lightLine();
  }
  
  // Check for an empty page
  for (i = 0; i < ptSize; i++){
    if (pt[process][i] == PT_NULL)
      return i;
  }
  
  // Check for the least recently used page
  for (i = 0; i < ptSize; i++) {
    if (ptCount[process][i] < min) {
      min = ptCount[process][i];
      pageTableId = i;
    }
  }
  
  return pageTableId;
}

// returns physical page id
int pt_loadPage(int process, int virtualPageId) {
  int i;
  int pageTableId;
  int physicalAddress;
  int* page;
  
  if (PT_DEBUG) {
    lightLine();
    printf("pt_loadPage(%d, %d)\n", process, virtualPageId);
    lightLine();
  }
  
  pageTableId = pt_evictPage(process);
  physicalAddress = pageTableId * PAGE_SIZE;
  
  page = fs_getPage(currentProgramId, process, virtualPageId * PAGE_SIZE, PAGE_SIZE);
  
  // update page table
  pt[process][pageTableId] = virtualPageId;
  
  // update the count
  ptCount[process][pageTableId] = ptCounter++;
  
  // load into physical memory
  for (i = 0; i < PAGE_SIZE; i++)
    mem[process][physicalAddress + i] = page[i];
  
  return pageTableId;
}

// returns the requested instruction
int pt_getInstruction(int process, int virtualAddress) {
  int physicalAddress;
  
  int virtualPage;
  int physicalPage;

  int offset;
  
  int instruction;
  
  if (PT_DEBUG) {
    heavyLine();
    printf("pt_requestInstruction(%d, %d)\n", process, virtualAddress);
    heavyLine();
  }
  
  virtualPage  = pt_getPageNumber(virtualAddress);
  physicalPage = pt_getPage(process, virtualPage);
  offset       = pt_getPageOffset(virtualAddress);
  
  if (physicalPage == PT_MISS) 
    physicalPage = pt_loadPage(process, virtualPage);

  
  physicalAddress = (physicalPage * PAGE_SIZE) + offset;
  instruction = mem[process][physicalAddress];

  if (PT_DEBUG) {
    lightLine();
    printf("instruction: %d\n", instruction);
    lightLine();
  }
  
  return instruction;  
}
