#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "filesystem2.h"



void pt_initialize();
void pt_dump();

int pt_getPageNumber(int instruction);
int pt_getPageOffset(int instruction);
int pt_getPage(int process, int vPage);
int pt_getInstruction(int process, int vAddress);
int pt_evictPage(int process);
int pt_loadPage(int process, int page);


