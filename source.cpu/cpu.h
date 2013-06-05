#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "helper.c"

#include "cpu.tab.h"

#include "scheduler.h"
#include "pagetable.h"
#include "filesystem2.h"

/* prototypes */
int exe(int stack[][STACKSIZE], int sp[], int reg[][REGISTERSIZE], int next_instruct[], int next_inst[], int cur_proc);
int pop(int stack[][STACKSIZE], int proc_id, int sp[], int calledfrom);
void push(int stack[][STACKSIZE], int proc_id, int sp[],int data, int calledfrom);
void print_stack(int stack[][STACKSIZE],int sp[]); //debug
void print_register(int reg[][REGISTERSIZE]); //debug
void print_gmem();
