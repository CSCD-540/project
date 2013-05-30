#include <stdio.h>
#include <stdlib.h>

#include "helper.h"

#define PT_DEBUG   1

#define MAXPRO     6
#define MAXMEM    16

#define PAGE_SIZE  4

#define PT_MISS   -1

void pt_initialize();
void pt_dump();
/*
void pt_loadPage(int process, int page);
int  pt_evictPage(int process);
int  pt_getVirtualPage(int process, int page);
*/