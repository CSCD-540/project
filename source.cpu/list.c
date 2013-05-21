#include "list.h"

List* list_create(char* name) {
  List* list = calloc(1, sizeof(List));
  list->name = name;
  
  return list;
}

void list_destroy(List* list, void(*destroyNode)(void* value)) {
  LIST_FOREACH(list, first, next, cur) {
    destroyNode(cur->value);
    if (cur->prev) 
      free(cur->prev);
  }
  
  free(list->last);
  free(list);
}

void list_clear(List* list) {
  LIST_FOREACH(list, first, next, cur)
    free(cur->value);
}

void* list_removeNode(List* list, ListNode* node) {
  void* result = NULL;
  
  if (node == list->first && node == list->last) {
    list->first = NULL;
    list->last = NULL;
  } else if (node == list->first) {
    list->first = node->next;
    list->first->prev = NULL;
  } else if (node == list->last) {
    list->last = node->prev;
    list->last->next = NULL;
  } else {
    ListNode* after = node->next;
    ListNode* before = node->prev;
    after->prev = before;
    before->next = after;
  }
  
  list->count--;
  result = node->value;
  free(node);
  
  return result;
}

void list_addFirst(List* list, void* value) {
  ListNode* node = calloc(1, sizeof(ListNode));

  node->value = value;

  if (LIST_DEBUG)
    printf("node->value: %s \t value: %s \n", (char*)node->value, (char*)value);
  
  if (list->first == NULL) {
    list->first = node;
    list->last = node;
  } else {
    node->next = list->first;
    list->first->prev = node;
    list->first = node;
  }
  
  list->count++;
}

void list_addLast(List* list, void* value) {
  ListNode* node = calloc(1, sizeof(ListNode));
  
  node->value = value;
  
  if (LIST_DEBUG)
    printf("node->value: %s \t value: %s \n", (char*)node->value, (char*)value);
  
  if (list->last == NULL) {
    list->first = node;
    list->last = node;
  } else {
    list->last->next = node;
    node->prev = list->last;
    list->last = node;
  }
  
  list->count++;
}

void* list_peekFirst(List* list) {
  ListNode* node = list->first;
  
  return node->value;
}


void* list_peekLast(List* list) {
  ListNode* node = list->last;
  
  return node->value;
}


void* list_peekNode(List* list, void* value, int (*compareNode)(void* valueA, void* valueB)) {
  ListNode* node = NULL;
  
  if (LIST_DEBUG)
    printf("node->value: %s \t value: %s \n", (char*)node->value, (char*)value);

  LIST_FOREACH(list, first, next, cur) {
    if (compareNode(cur->value, value) == 0) {
      node = cur;
      break;
    }
  }
  
  if (node != NULL)
    return node->value;
    
  return NULL;
}


void* list_popFirst(List* list) {
  ListNode* node = list->first;
  
  if (node != NULL)
    return list_removeNode(list, node);
  
  return NULL;
}


void* list_popLast(List* list) {
  ListNode* node = list->last;
  
  if (node != NULL)
    return list_removeNode(list, node);
  
  return NULL;
}


void* list_popNode(List* list, void* value, int (*compareNode)(void* nodeType, void* value)) {
  ListNode* node = NULL;
  
  if (LIST_DEBUG)
    printf("node->value: %s \t value: %s \n", (char*)node->value, (char*)value);

  LIST_FOREACH(list, first, next, cur) {
    if (compareNode(cur->value, value) == 0) {
      node = cur;
      break;
    }
  }
  
  if (node != NULL)
    return list_removeNode(list, node);

  return NULL; 
}

void list_print(List* list, void (*printNode)(void* node)) {
  printf("==================================================\n");
  printf("LIST: %s \n", list->name);

  LIST_FOREACH(list, first, next, cur)
    printNode(cur->value);

  printf("--------------------------------------------------\n");
}

int list_valueIsUnique(List* list, void* value, int (*compareNode)(void* nodeType, void* value)) {
  int result = TRUE;
  
  LIST_FOREACH(list, first, next, cur) {
    if (compareNode(cur->value, value) == 0) {
      result = FALSE;
      break;
    }
  }
  
  return result;
}