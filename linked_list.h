#ifndef linked_list_h
#define linked_list_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Variable length list_t management with pointers with DICCIONARY behaviour.

#define FAILURE 0
#define SUCCESS 1

#define MAX 64
#define SIZE l->size

typedef struct {
  int b; // body
  int c; // count
} bd_t;

typedef struct node_t {
  bd_t * body;
  struct node_t * next;
} node_t;

typedef struct {
  node_t* head;
  node_t* tail;
  int   size;
} list_t;

/* ------- PROTOTYPES ------- */
int create(list_t*);
int append(list_t*, int);
int append_unique(list_t*, int);
int append_unique_list(list_t*, int*, int);
int in(list_t*, int);
int remove_(list_t*, int);
int remove_list(list_t*, int*, int);
int print_list(list_t*);
int del(list_t*);
int save_list(list_t*, char* fname, int offset);
int open_list(list_t*, char* fname, int offset);
/* -------------------------- */

int create(list_t* l){
  l->head = NULL;
  l->tail = NULL;
  SIZE = 0;
  return SUCCESS;
}

int append(list_t* l, int elem, int count){
  if (l == NULL) return FAILURE;
  if (SIZE == MAX) return FAILURE;
  node_t* nodo;
  nodo = malloc(sizeof(node_t));
  if (nodo == NULL) return FAILURE;
  nodo->body = malloc(sizeof(bd_t));
  if (nodo->body == NULL) return FAILURE;
  nodo->body->b = elem;
  nodo->body->c = count;
  nodo->next = NULL;
  if (SIZE > 0) {
    l->tail->next = malloc(sizeof(node_t));
    if (l->tail->next == NULL) return FAILURE;
    l->tail->next = nodo;
  } else {
    l->head = nodo;
  }
  l->tail = nodo;
  SIZE++;
  return SUCCESS;
}

int append_unique(list_t* l, int elem){
  if (l == NULL) return FAILURE;
  if (SIZE == MAX) return FAILURE;
  node_t* i = l->head;
  while (1) {
    if (i == NULL) { append(l,elem,1); break;}
    if (i->body->b == elem) { i->body->c++; break;}
    i = i->next;
  }
  return SUCCESS;
}

int append_unique_list(list_t* l, int* array, int sz){
  if (l == NULL) return FAILURE;
  for (size_t i = 0; i < sz; i++) {
    if (!append_unique(l, array[i])) return FAILURE;
  }
  return SUCCESS;
}

int remove_(list_t* l, int elem){
  if (l == NULL) return FAILURE;
  node_t* i = l->head;
  node_t* j = i;
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

int remove_list(list_t* l, int* array, int sz){
  if (l == NULL) return FAILURE;
  for (size_t i = 0; i < sz; i++) {
    if (!remove_(l, array[i])) return FAILURE;
  }
  return SUCCESS;
}

int in(list_t* l, int elem){
  if (l == NULL) return FAILURE;
  node_t* i = l->head;
  while (1) {
    if (i == NULL) break;
    if (i->body->b == elem) return SUCCESS;
    i = i->next;
  }
  return FAILURE;
}

int print_list(list_t* l){
  if (l == NULL) return FAILURE;
  node_t* i = l->head;
  while (1) {
    if (i == NULL) break;
    printf("%2i | %i\n", i->body->b, i->body->c);
    i = i->next;
  }
  return SUCCESS;
}

int del(list_t* l) {
  if (l == NULL) return FAILURE;
  node_t* i = l->head;
  node_t* j;
  while (1) {
    if (i == NULL) break;
    j = i->next;
    free(i);
    i = j;
  }
  return SUCCESS;
}

int save_list(list_t* l, char* fname, int* offset){
  if (l == NULL) return FAILURE;
  node_t* i = l->head;
  int c = 0;
  FILE* fp = fopen(fname, "wb");
  fseek(fp, offset, SEEK_SET);
  fwrite(l.size, sizeof(int), 1, fp);
  offset += sizeof(int);
  while (1) {
    offset += c * sizeof(bd_t);
    if (i == NULL) break;
    fseek(fp, offset, SEEK_SET);
    fwrite(i->body, sizeof(bd_t), 1, fp);
    i = i->next;
    c++;
  }
  fclose(fp);
}

int open_list(list_t* l, char* fname, int offset){
  create_list(l);
  int sz;
  FILE* fp = fopen(fname, "rb");
  // read list size value
  fseek(fp, offset, SEEK_SET);
  fread(sz, sizeof(int), 1, fp);
  offset += sizeof(int);
  for (size_t i = 0; i < sz; i++) {
    bd_t body;
    fseek(fp, offset +  i * sizeof(bd_t), SEEK_SET);
    fread(body, sizeof(bd_t), 1, fp);
    append(l, body.b, body.c);
  }
  fclose(fp);
}

#endif
