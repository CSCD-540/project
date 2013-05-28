#include <stdio.h>
#include <stdlib.h>

#define DEBUG 0
#define TRUE  1
#define FALSE 0

char getChar(FILE* fp);
char* getString(FILE* fp);
int getInt(FILE* fp);

void heavyLine();
void lightLine();
void starLine();