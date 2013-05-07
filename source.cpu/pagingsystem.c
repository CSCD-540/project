#define PT_VIRTUAL_PAGES 200        // max size of page table

int pageTable[MAXPRO][PT_VIRTUAL_PAGES];
int pageTableLRU[MAXPRO][MAXPRO];         // used to track the last time a page was used
int pageTableLRUCounter = 0;                 // 


void pageTable_Initialize();
void pageTable_LoadPage(int process, int page);
int pageTable_ReplacementPolicy(int process);
int pageTable_getVirtualPage(int process, int page);

void pageTable_Initialize() {
  int i;
  int j;
  
  for (i = 0; i < MAXPRO; i++)
    for (j = 0; j < PT_VIRTUAL_PAGES; j++)
      pageTable[i][j] = -1;
}

void pageTable_LoadPage(int process, int page) {
  int virtualPage;
  
  virtualPage = pageTable_ReplacementPolicy(process);
  pageTable[process][virtualPage] = page;
  pageTableLRU[process][virtualPage] = pageTableLRUCounter++;
}

// -1 indicates page fault
int pageTable_getVirtualPage(int process, int page) {
  int virtualPage = -1;
  int i;
  
  for (i = 0; i < PT_VIRTUAL_PAGES; i++) 
    if (pageTable[process][i] == page)
      virtualPage = i;
  
  return i;
  
}

int pageTable_ReplacementPolicy(int process) {
  int i;
  int virtualPage = -1;
  int min = pageTableLRUCounter;
  
  // Check for empty page
  for (i = 0; i < PT_VIRTUAL_PAGES; i++)
    if (pageTable[process][i] == -1)
      return i;
  
  // Check for least recently used page
  for (i = 0; i < PT_VIRTUAL_PAGES; i++)
    if (pageTableLRU[process][i] < min) {
      min = pageTableLRU[process][i];
      virtualPage = i;
    }
  
  return virtualPage;
}