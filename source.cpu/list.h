#ifndef LIST_H_INCLUDED
  #define LIST_H_INCLUDED
  
  #include <stdlib.h>
  #include <stdio.h>
  #include <string.h>

  #define LIST_DEBUG 0
  #define TRUE 1
  #define FALSE 0

  #define LIST_COUNT(A) ((A)->count)
  #define LIST_FIRST(A) ((A)->first != NULL ? (A)->first->value : NULL)
  #define LIST_LAST(A)  ((A)->last != NULL ? (A)->last->value : NULL)
  #define LIST_FOREACH(L, S, M, V) ListNode *_node = NULL; \
    ListNode *V = NULL; \
      for (V = _node = L->S; _node != NULL; V = _node = _node->M)


  typedef struct ListNode {
    struct ListNode *next;
    struct ListNode *prev;
    void* value;
  } ListNode;

  typedef struct List {
    char* name;
    int   count;
    ListNode* first;
    ListNode* last;
  } List;
  
  List* list_create(char* name);
  void  list_destroy();
  void  list_clear();
  void* list_removeNode(List* list, ListNode* node);

  void  list_addFirst(List* list, void* value);
  void  list_addLast(List* list, void* value);

  void* list_peekFirst(List* list);
  void* list_peekLast(List* list);
  void* list_peekNode(List* list, void* value, int (*compareNode)(void* nodeType, void* value));

  void* list_popFirst(List* list);
  void* list_popLast(List* list);
  void* list_popNode(List* list, void* value, int (*compareNode)(void* nodeType, void* value));

  void list_print(List* list, void (*printNode)(void* node));
  int  list_valueIsUnique(List* list, void* value, int (*compareNode)(void* nodeType, void* value));

#endif