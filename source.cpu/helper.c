#include "helper.h"

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