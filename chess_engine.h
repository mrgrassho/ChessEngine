#ifndef chess_engine_h
#define chess_engine_h

#include <stdio.h>
#include <stdint.h>
#include "linked_list.h"

#define PIECES 16
#define BRD_LEN 64

enum type_piece_t{
  PAWN,
  TOWER,
  HORSE,
  BISHOP,
  QUEEN,
  KING
};
typedef enum type_piece_t type_piece;

const char* type_ch[] = {"PAWN", "TOWER", "HORSE", "BISHOP", "QUEEN", "KING"};

enum color_t{
  BLACK,
  WHITE
};
typedef enum color_t color;

const char* color_ch[] = {"BLACK", "WHITE"};

struct piece_t{
  type_piece tp;
  color cl;
};
typedef struct piece_t piece;

struct cell_t{
  piece* p;
};
typedef struct cell_t cell;

// KING IN CHECK LIST - Positions where the enemy's king CAN'T be in.
list jql_white;
list jql_black;

#define FAILURE 0
#define SUCCESS 1
#define NOPIECE 2

int init_brd(cell * brd){
  create_list(&jql_black);
  create_list(&jql_white);
  // PAWNS
  for (size_t i = 0; i < 64; i++) {
    brd->p = malloc(sizeof(piece));
    if (brd == NULL) return FAILURE;
    brd++;
  }
  brd -= 64;  // IMPORTANT -> step back 64 instructions.
  for (int i = 8; i < 16; i++) {
    brd[i].p->tp = PAWN;
    brd[i].p->cl = BLACK;
  }
  for (int i = 48; i < 56; i++) {
    brd[i].p->tp = PAWN;
    brd[i].p->cl = WHITE;
  }
  // HORSES
  brd[1].p->tp = HORSE;
  brd[1].p->cl = BLACK;
  brd[6].p->tp = HORSE;
  brd[6].p->cl = BLACK;
  brd[57].p->tp = HORSE;
  brd[57].p->cl = WHITE;
  brd[62].p->tp = HORSE;
  brd[62].p->cl = WHITE;
  // TOWER
  brd[0].p->tp = TOWER;
  brd[0].p->cl = BLACK;
  brd[7].p->tp = TOWER;
  brd[7].p->cl = BLACK;
  brd[56].p->tp = TOWER;
  brd[56].p->cl = WHITE;
  brd[63].p->tp = TOWER;
  brd[63].p->cl = WHITE;
  // BISHOP
  brd[2].p->tp = BISHOP;
  brd[2].p->cl = BLACK;
  brd[5].p->tp = BISHOP;
  brd[5].p->cl = BLACK;
  brd[58].p->tp = BISHOP;
  brd[58].p->cl = WHITE;
  brd[61].p->tp = BISHOP;
  brd[61].p->cl = WHITE;
  // QUEENS
  brd[3].p->tp = QUEEN;
  brd[3].p->cl = BLACK;
  brd[59].p->tp = QUEEN;
  brd[59].p->cl = WHITE;
  // KINGS
  brd[4].p->tp = KING;
  brd[4].p->cl = BLACK;
  brd[60].p->tp = KING;
  brd[60].p->cl = WHITE;
  // REST OF THE CHESSBOARD SET TO NULL
  for (size_t i = 16; i < 48; i++) {
    brd[i].p = NULL;
  }
  brd -= 64;  // IMPORTANT -> step back 64 instructions.
  return SUCCESS;
}

int get_index(uint8_t ch, uint8_t num, int * i){
  if ((ch < 65) || (ch > 72)) {
    return FAILURE;
  }
  if ((ch < 31) || (ch > 38)) {
    return FAILURE;
  }
  ch -= 65;
  num -= 31;
  *i = 8 * (8 - num) + ch;
  return SUCCESS;
}

int get_coord(int i, char* nd){
  if ((i < 0) || (i > 63)) {
    return FAILURE;
  }
  nd[0] = (char) 65 + i % 8;
  nd[1] = (char) 31 + i / 8;
  return SUCCESS;
}

int is_valid_index(int i){
  if ((i < 0) || (i > 63)) return FAILURE;
  return SUCCESS;
}

int is_empty(cell * brd, int i){
  if (!is_valid_index(i)) return FAILURE;
  return brd[i].p == NULL;
}

int is_enemy(cell * brd, int i, color cl){
  if (!is_valid_index(i)) return FAILURE;
  if (!is_empty(brd, i)) return brd[i].p->cl != cl;
  return FAILURE;
}

const char* get_type(cell* brd, int p){
  if ((p < 0) || (p > 63)) return "";
  if (is_empty(brd, p)) return "";
  return type_ch[brd[p].p->tp];
}

const char* get_color(cell* brd, int p){
  if ((p < 0) || (p > 63)) return "";
  if (is_empty(brd, p)) return "";
  return color_ch[brd[p].p->cl];
}

// ONLY FOR PAWNS
int _get_next(cell * brd, int i, int stps){
  if (brd[i].p->cl == WHITE)
    return i - stps;
  else
    return i + stps;
}

int _same_row(int d, int k){
  if (!is_valid_index(d) || !is_valid_index(k)) return FAILURE;
  return d/8 == k/8;
}

int _same_column(int d, int k){
  if (!is_valid_index(d) || !is_valid_index(k)) return FAILURE;
  return d%8 == k%8;
}

int get_movlist_pawns(cell * brd, int i, int* l, int* sz){
  if (!is_valid_index(i)) return FAILURE;
  if (brd == NULL) return FAILURE;

  int k = _get_next(brd,i,8);
  if (!is_valid_index(k))
    return FAILURE;
  else {
    if (is_empty(brd, k)) {
      *l++ = k;
      *sz += 1;
      if ((i / 8 == 1) || (i / 8 == 6)) {
        int j = _get_next(brd,i,16);
        if (is_valid_index(j))
          if (is_empty(brd, j))
            *l++ = j;
            *sz += 1;
      }
    }
    if (_same_row(i,k-1)) {
      if (is_enemy(brd, k-1, brd[i].p->cl)) {
        *l++ = k-1;
        *sz += 1;
      }
    }
    if (_same_row(i,k+1)) {
      if (is_enemy(brd, k+1, brd[i].p->cl)) {
        *l++ = k+1;
        *sz += 1;
      }
    }
  }
  return SUCCESS;
}

int get_movlist_towers(cell * brd, int i, int* l, int* sz){
  if (!is_valid_index(i)) return FAILURE;
  if (brd == NULL) return FAILURE;
  // going right
  if(_same_row(i, i+1)){
    for (size_t j = (i % 8) + 1; j < 8; j++) {
      if (is_empty(brd,i+j) || is_enemy(brd,i+j,brd[i].p->cl)) {*l++ = i+j; *sz += 1;}
      else break;
    }
  }
  // going left
  if(_same_row(i, i-1)){
    for (size_t j = (i % 8) - 1; j > -1; j--) {
      if (is_empty(brd,i-j) || is_enemy(brd,i-j,brd[i].p->cl)) {*l++ = i-j; *sz += 1;}
      else break;
    }
  }
  // going up
  if(_same_column(i, i+8)){
    for (size_t j = i + 8; j < 64; j+=8) {
      if (is_empty(brd,j) || is_enemy(brd,j,brd[i].p->cl)) {*l++ = j; *sz += 1;}
      else break;
    }
  }
  // going down
  if(_same_column(i, i-8)){
    for (size_t j = i-8; j > -1; j-=8) {
      if (is_empty(brd,j) || is_enemy(brd,j,brd[i].p->cl)) {*l++ = j; *sz += 1;}
      else break;

    }
  }
  return SUCCESS;
}

int _aux_horses(cell * brd, int i, int k, int q, int* l, int* sz){
  int m; int n;  int o;  int p;
  if (_same_row(i, k)) {
    if (_same_column(k, m = k-16)) {
      if (is_empty(brd, m) || is_enemy(brd,m,brd[i].p->cl)){
        *l++ = m; *sz += 1;
      }
    }
    if (_same_column(k, o = k+16)) {
      if (is_empty(brd, o) || is_enemy(brd,o,brd[i].p->cl)){
        *l++ = o; *sz += 1;
      }
    }
  }
  if (_same_row(i, q)) {
    if (_same_column(q, n = q-8)) {
      if (is_empty(brd, n) || is_enemy(brd,n,brd[i].p->cl)){
        *l++ = n; *sz += 1;
      }
    }
    if (_same_column(q, p = q+8)) {
      if (is_empty(brd, p) || is_enemy(brd,p,brd[i].p->cl)) {
        *l++ = p; *sz += 1;
      }
    }
  }
  return 0;
}

int get_movlist_horses(cell * brd, int i, int* l, int* sz){
  if (!is_valid_index(i)) return FAILURE;
  if (brd == NULL) return FAILURE;
  _aux_horses(brd, i, i+1, i+2, l, sz);
  l += *sz; //IMPORTANT - SHIFTS MEMORY POINTER
  _aux_horses(brd, i, i-1, i-2, l, sz);
  return 0;
}

int get_movlist_bishops(cell * brd, int i, int* l, int* sz){
  if (!is_valid_index(i)) return FAILURE;
  if (brd == NULL) return FAILURE;
  // going right up
  for (size_t j = i - 7; i > -1; i -= 7){
    if (is_empty(brd,j) || is_enemy(brd,j,brd[i].p->cl)) {*l++ = j; *sz += 1;}
    else break;
  }
  // going right down
  for (size_t j = i + 9; i < 64; i += 9) {
    if (is_empty(brd,j) || is_enemy(brd,j,brd[i].p->cl)) {*l++ = j; *sz += 1;}
    else break;
  }
  // going left down
  for (size_t j = i + 7; i < 64; i += 7) {
    if (is_empty(brd,j) || is_enemy(brd,j,brd[i].p->cl)) {*l++ = j; *sz += 1;}
    else break;
  }
  // going left up
  for (size_t j = i - 9; i > -1; i -= 9){
    if (is_empty(brd,j) || is_enemy(brd,j,brd[i].p->cl)) {*l++ = j; *sz += 1;}
    else break;
  }
  return SUCCESS;
}

int convert_index_4_king(int p, int i, int* k){
  if (is_valid_index(p)) return FAILURE;
  if (i < 0 || i > 9) return FAILURE;

  if (i < 3)
    if (!is_valid_index(*k = (p - 9) + i)) *k = -1;

  if (i < 6)
    if (!is_valid_index(*k = (p - 1) + (i % 3))) *k = -1;

  if (i < 9)
    if (!is_valid_index(*k = (p + 7) + (i % 3))) *k = -1;

  return SUCCESS;
}

int get_movlist_kings(cell * brd, int p, int* l, int* sz){
  if (!is_valid_index(p)) return FAILURE;
  if (brd == NULL) return FAILURE;
  int k; list jql; int answer;
  if (brd[p].p->cl == WHITE) jql = jql_white;
  else jql = jql_black;
  for (int i = 0; i < 9; i++) {
    if (convert_index_4_king(p, i, &k)){
      if (k != -1) {
        if (!in_list(&jql, k))
          if (is_empty(brd,k) || is_enemy(brd,k,brd[k].p->cl)) {*l++ = k; *sz += 1;}
      }
    }
  }
  return SUCCESS;
}

int get_movlist(cell * brd, int p, int* l, int* sz){
  if (brd == NULL) return FAILURE;
  if (!is_valid_index(p)) return FAILURE;
  if (is_empty(brd,p)) return NOPIECE;
  *sz = 0;                            // Size of the list
  switch (brd[p].p->tp) {
    case PAWN:
      if (!get_movlist_pawns(brd, p, l, sz)) return FAILURE;
      break;
    case TOWER:
      if (!get_movlist_towers(brd, p, l, sz)) return FAILURE;
      break;
    case HORSE:
      if (!get_movlist_horses(brd, p, l, sz)) return FAILURE;
      break;
    case QUEEN:
      if (!get_movlist_towers(brd, p, l, sz) || !get_movlist_bishops(brd, p, l, sz)) return FAILURE;
      break;
    case BISHOP:
      if (!get_movlist_bishops(brd, p, l, sz)) return FAILURE;
      break;
    case KING:
      if (!get_movlist_kings(brd, p, l, sz)) return FAILURE;
      break;
  }
  return SUCCESS;
}

/*int move_pierce(cell * brd, int p, int q){
  if (brd == NULL) return FAILURE;
  if (!is_valid_index(p)) return FAILURE;
  if (!is_valid_index(q)) return FAILURE;
}*/

#endif
