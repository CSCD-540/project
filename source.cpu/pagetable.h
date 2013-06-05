#include <stdio.h>
#include <stdlib.h>

#include "config.h"

#define PT_DEBUG   1

#define PAGE_SIZE  4

#define PT_MISS   -1

void pt_initialize();
void pt_dump();

int pt_getPageNumber(int instruction);
int pt_getPageOffset(int instruction);
int pt_getPage(int process, int vPage);
int pt_requestInstruction(int process, int vAddress);
int pt_evictPage(int process);
int pt_loadPage(int process, int page);


