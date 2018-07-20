#ifndef linked_list_h
#define linked_list_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Variable length List management with pointers with SET behaviour.

#define FAILURE 0
#define SUCCESS 1
#define MAXSIZE 2
#define MAX 32
#define SIZE l->size

struct node_t {
  int body;
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
int create_list(list*);
int append_list(list*, int);
int append_unique_list(list*, int);
int remove_list(list*, int);
int print_list(list*);
int del_list(list*);
/* -------------------------- */

int create_list(list* l){
  l = malloc(sizeof(list));
  if (l == NULL) return FAILURE;
  SIZE = 0;
  return SUCCESS;
}

int append_list(list* l, int elem){
  if (l == NULL) return FAILURE;
  if (SIZE == MAX) return MAXSIZE;
  node* nodo;
  nodo = malloc(sizeof(node));
  if (nodo == NULL) return FAILURE;
  nodo->body = elem;
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

int append_unique_list(list* l, int elem){
  if (l == NULL) return FAILURE;
  if (SIZE == MAX) return MAXSIZE;
  node* i = l->head;
  while (1) {
    if (i == NULL) { append_list(l,elem); break;}
    if (i->body == elem) break;
    i = i->next;
  }
  return SUCCESS;
}

int remove_list(list* l, int elem){
  if (l == NULL) return FAILURE;
  node* i = l->head;
  node* j = i;
  while (1) {
    if (i == NULL) break;
    if (i->body == elem) {
      if (l->head == l->tail) { del_list(l); create_list(l); }
      else {
        if (l->head == i) l->head = l->head->next;
        if (l->tail == i) l->tail->next = NULL;
        j->next = i->next;
        free(i);
        SIZE--;
        break;
      }
    }
    j = i;
    i = i->next;
  }
  return SUCCESS;
}

int in_list(list* l, int elem){
  if (l == NULL) return FAILURE;
  node* i = l->head;
  while (1) {
    if (i == NULL) break;
    if (i->body == elem) return SUCCESS;
    i = i->next;
  }
  return FAILURE;
}

int print_list(list* l){
  if (l == NULL) return FAILURE;
  node* i = l->head;
  while (1) {
    if (i == NULL) break;
    printf("%i\n", i->body);
    i = i->next;
  }
  return SUCCESS;
}

int del_list(list* l) {
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
