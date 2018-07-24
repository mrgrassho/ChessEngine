#ifndef linked_list_h
#define linked_list_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Variable length List management with pointers with DICCIONARY behaviour.

#define FAILURE 0
#define SUCCESS 1

#define MAX 64
#define SIZE l->size

struct bd_t {
  int b; // body
  int c; // count
};

struct node_t {
  struct bd_t* body;
  struct node_t* next;
};
typedef struct node_t node;

struct list_t {
  node* head;
  node* tail;
  int   size;
};
typedef struct list_t list;

/* ------- PROTOTYPES ------- */
int create(list*);
int append(list*, int);
int append_unique(list*, int);
int append_unique_list(list*, int*, int);
int in(list*, int);
int remove_(list*, int);
int remove_list(list*, int*, int);
int print_list(list*);
int del(list*);
/* -------------------------- */

int create(list* l){
  l->head = NULL;
  l->tail = NULL;
  SIZE = 0;
  return SUCCESS;
}

int append(list* l, int elem){
  if (l == NULL) return FAILURE;
  if (SIZE == MAX) return FAILURE;
  int count = 1;
  node* nodo;
  nodo = malloc(sizeof(node));
  if (nodo == NULL) return FAILURE;
  nodo->body = malloc(sizeof(struct bd_t));
  if (nodo->body == NULL) return FAILURE;
  nodo->body->b = elem;
  nodo->body->c = count;
  nodo->next = NULL;
  if (SIZE > 0) {
    l->tail->next = malloc(sizeof(node));
    if (l->tail->next == NULL) return FAILURE;
    l->tail->next = nodo;
  } else {
    l->head = nodo;
  }
  l->tail = nodo;
  SIZE++;
  return SUCCESS;
}

int append_unique(list* l, int elem){
  if (l == NULL) return FAILURE;
  if (SIZE == MAX) return FAILURE;
  node* i = l->head;
  while (1) {
    if (i == NULL) { append(l,elem); break;}
    if (i->body->b == elem) { i->body->c++; break;}
    i = i->next;
  }
  return SUCCESS;
}

int append_unique_list(list* l, int* array, int sz){
  if (l == NULL) return FAILURE;
  for (size_t i = 0; i < sz; i++) {
    if (!append_unique(l, array[i])) return FAILURE;
  }
  return SUCCESS;
}

int remove_(list* l, int elem){
  if (l == NULL) return FAILURE;
  node* i = l->head;
  node* j = i;
  while (1) {
    if (i == NULL) break;
    if (i->body->b == elem) {
      if (l->head == l->tail && i->body->c == 1) { del(l); create(l); }
      else {
        if (i->body->c == 1){
          if (l->head == i) l->head = l->head->next;
          if (l->tail == i) l->tail->next = NULL;
          j->next = i->next;
          free(i);
          SIZE--;
        } else i->body->c--;
        break;
      }
    }
    j = i;
    i = i->next;
  }
  return SUCCESS;
}

int remove_list(list* l, int* array, int sz){
  if (l == NULL) return FAILURE;
  for (size_t i = 0; i < sz; i++) {
    if (!remove_(l, array[i])) return FAILURE;
  }
  return SUCCESS;
}

int in(list* l, int elem){
  if (l == NULL) return FAILURE;
  node* i = l->head;
  while (1) {
    if (i == NULL) break;
    if (i->body->b == elem) return SUCCESS;
    i = i->next;
  }
  return FAILURE;
}

int print_list(list* l){
  if (l == NULL) return FAILURE;
  node* i = l->head;
  while (1) {
    if (i == NULL) break;
    printf("%2i | %i\n", i->body->b, i->body->c);
    i = i->next;
  }
  return SUCCESS;
}

int del(list* l) {
  if (l == NULL) return FAILURE;
  node* i = l->head;
  node* j;
  while (1) {
    if (i == NULL) break;
    j = i->next;
    free(i);
    i = j;
  }
  return SUCCESS;
}

#endif
