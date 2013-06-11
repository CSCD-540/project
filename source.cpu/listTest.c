#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "inode.h"


void char_destroy() {
  ;
}

void char_print(void* value) {
  if (value == NULL)
    return;
    
  printf("%s \n", (char*)value);
}

int char_compare(void* nodeA, void* nodeB) {
  return strcmp((char*)nodeA, (char*)nodeB);
}


int main() {  
  List* list = list_create("Char List");

  printf("empty list...\n");
  list_print(list, char_print);
  
  printf("\n\nadding strings...\n");
  list_addLast(list, "andrew");
  list_addLast(list, "barry");
  list_addLast(list, "chuck");
  
  list_addFirst(list, "zumba");
  list_addFirst(list, "kai");
  
  printf("\n\nunique value \"bob\"...\n");
  printf("%d \n", list_valueIsUnique(list, "bob", char_compare));
  
  printf("\n\nduplicate \"kai\"...\n");
  printf("%d \n", list_valueIsUnique(list, "kai", char_compare));
  
  printf("\n\ndisplaying list...\n");
  printf("first: %s \n", (char*)list->first->value);
  printf("last:  %s \n", (char*)list->last->value);
  list_print(list, char_print);

  printf("\n\npeeking first... \n");
  printf("%s \n", (char*)list_peekFirst(list));
  
  printf("\n\npeeking last...\n");
  printf("%s \n", (char*)list_peekLast(list));
  
  printf("\n\npeeking by string comparison with \"kai\"...\n");
  printf("%s \n", (char*)list_peekNode(list, "kai", char_compare));
  
  printf("\n\ndisplaying list...\n");
  printf("first: %s \n", (char*)list->first->value);
  printf("last:  %s \n", (char*)list->last->value);
  list_print(list, char_print);
  
  printf("\n\npoppining first... \n");
  printf("%s \n", (char*)list_popFirst(list));
  
  printf("\n\npopping last...\n");
  printf("%s \n", (char*)list_popLast(list));
  
  printf("\n\npopping by string comparison with \"andrew\"...\n");
  printf("%s \n", (char*)list_popNode(list, "andrew", char_compare));
  
  printf("\n\npopping by string comparison with \"andrew\" (already removed)...\n");
  printf("%s \n", (char*)list_popNode(list, "andrew", char_compare));
  
  printf("\n\ndisplaying list...\n");
  printf("first: %s \n", (char*)list->first->value);
  printf("last:  %s \n", (char*)list->last->value);
  list_print(list, char_print);
  
  
  printf("\n\ndestroying list...\n\n\n");
  list_destroy(list, char_destroy);
  
  /* ----------------------------------------- */

  list = list_create("INode List");
  
  printf("\n\nadding inode...\n");
  
  INode* n1 = inode_create(1, "andrew", 0, 20);
  list_addLast(list, n1);

  INode* n2 = inode_create(2, "chuck", 21, 30);
  list_addLast(list, n2);
  
  INode* n3 = inode_create(3, "kai", 51, 10);
  list_addFirst(list, n3);
  
  INode* n4 = inode_create(4, "jessi", 62, 5);
  list_addLast(list, n4);

  printf("\n\nunique value id 99...\n");
  printf("%d \n", list_valueIsUnique(list, (void*)99, inode_compareById));
  
  printf("\n\nduplicate value id 1...\n");
  printf("%d \n", list_valueIsUnique(list, (void*)1, inode_compareById));

  printf("\n\ndisplaying list...\n");
  list_print(list, inode_print);

  INode* n;

  printf("\n\npeeking first...\n");
  n = list_peekFirst(list);
  inode_print(n);

  printf("\n\npeeking last...\n");
  n = list_peekLast(list);
  inode_print(n);
  
  printf("\n\npeeking by string comparison with 3...\n");
  n = list_peekNode(list, (void*)3, inode_compareById);
  inode_print(n);
  
  printf("\n\ndisplaying list...\n");
  list_print(list, inode_print);

  printf("\n\npopping first...\n");
  n = list_popFirst(list);
  inode_print(n);
  inode_destroy(n);
  

  printf("\n\npopping last...\n");
  n = list_popLast(list);
  inode_print(n);
  inode_destroy(n);

  printf("\n\npopping by string comparison with 1...\n");
  n = list_popNode(list, (void*)1, inode_compareById);
  inode_print(n);
  inode_destroy(n);
  
  printf("\n\npopping by string comparison with 1 (already removed)...\n");
  n = list_popNode(list, (void*)1, inode_compareById);
  inode_print(n);
  inode_destroy(n);

  printf("\n\ndisplaying list...\n");
  list_print(list, inode_print);
  
  list_destroy(list, inode_destroy);
  

  return 0;
}