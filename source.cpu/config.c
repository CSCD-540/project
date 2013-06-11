#include "config.h"

int  gmem[MAXGMEM];         // global var sit here 
int  mem[MAXPRO][MAXMEM];   // physical memory

int  p0running;

int currentProgramId;

char getChar(FILE* fp) {
  char c;
  
  fscanf(fp, "%c", &c);
  if (DEBUG)
    printf("%c\n", c);
  
  return c;
}

char* getString(FILE* fp) {
  char* s;  
  s = (char*) calloc(48, sizeof(char));
  
  fscanf(fp, "%s", s);
  if (DEBUG)
    printf("%s\n", s);

  return s;
}

int getInt(FILE* fp) {
  int i;
  
  fscanf(fp, "%d", &i);
  
  if (DEBUG)
    printf("%d\n", i);
  
  return i;
}

void heavyLine() {
  printf("============================================================\n");
}

void lightLine() {
  printf("------------------------------------------------------------\n");
}

void starLine() {
  printf("************************************************************\n");
}

void dumpData(int size, int* data) {
  int i;
  
  printf("    ");
  for (i = 0; i < 20; i++)
    printf("%4d", i);
  printf("\n");

  for (i = 0; i < size; i++) {
    // row number
    if (i % 20 == 0) 
      printf("%4d", i / 20);
      
    printf("%4d", data[i]);
    
    // row line break
    if (i % 20 == 19)
      printf("\n");
  }
    
  printf("\n");
}