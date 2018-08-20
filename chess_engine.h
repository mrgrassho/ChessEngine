#ifndef chess_engine_h
#define chess_engine_h

#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include "linked_list.h"

#define PIECES 16
#define BRD_LEN 64

typedef enum {
  PAWN,
  TOWER,
  HORSE,
  BISHOP,
  QUEEN,
  KING
} type_piece_t;

const char* type_ch[] = {"PAWN", "TOWER", "HORSE", "BISHOP", "QUEEN", "KING"};

typedef enum {
  BLACK,
  WHITE
} color_t;

const char* color_ch[] = {"BLACK", "WHITE"};

typedef struct {
  type_piece_t tp;
  color_t cl;
} piece_t;


typedef struct {
  piece_t* p;
} cell_t;

// KING IN CHECK LIST - Positions where the enemy's king CAN'T be in.
list_t jql_white;
list_t jql_black;
list_t history;

typedef struct {
  enum {SAFE, JAQUE, JMATE} king_state;
  int indx;
} king_t;

king_t king_black;
king_t king_white;

color_t current_color;

typedef enum {
  FAILURE,
  SUCCESS,
  NOPIECE,
  FILE_NOT_EXIST
} error_chss_t;

error_chss_t save_chess(cell_t* brd, char* fname) {
  if (brd == NULL) return FAILURE;
  if (fname == NULL) return FAILURE;
  int offset = 0;
  FILE * fp = fopen(fname, "wb");
  // save BOARD
  for (size_t i = 0; i < 64; i++) {
    offset = sizeof(piece_t) * i;
    fseek(fp, offset, SEEK_SET);
    fwrite(brd->p, sizeof piece_t, 1, fp);
  }
  offset = sizeof(piece_t) * 64;
  // save jql_black
  save_list(&jql_black, fname, &offset);
  // save jql_white
  save_list(&jql_white, fname, &offset);
  // save history
  save_list(&history, fname, &offset);
  // save black king structure
  fseek(fp, offset, SEEK_SET);
  fwrite(king_black, sizeof(king_t), 1, fp);
  offset += sizeof(king_t);
  // save white king structure
  fseek(fp, offset, SEEK_SET);
  fwrite(king_white, sizeof(king_t), 1, fp);
  offset += sizeof(king_t);
  // save color (current turn)
  fseek(fp, offset, SEEK_SET);
  fwrite(current_color, sizeof(color_t), 1, fp);
  fclose(fp);
  return SUCESS;
}

error_chss_t open_chess(cell_t* brd, char* fname){
  if (brd != NULL) return FAILURE;
  if (fname == NULL) return FAILURE;
  FILE * fp = fopen(fname, "rb");
  if (!fp) return FILE_NOT_EXIST;
  int offset;
  // load BOARD
  for (size_t i = 0; i < 64; i++) {
    brd[i].p = malloc(sizeof(piece_t));
    if (brd[i].p == NULL) return FAILURE;
    offset = sizeof(piece_t) * i;
    fseek(fp, offset, SEEK_SET);
    fread(brd[i].p, sizeof(piece_t), 1, fp);
  }
  offset = sizeof(piece_t) * 64;
  // open jql_black
  open_list(&jql_black, fname, &offset);
  // open jql_white
  open_list(&jql_white, fname, &offset);
  // open history
  open_list(&history, fname, &offset);
  // save black king structure
  fseek(fp, offset, SEEK_SET);
  fread(king_black, sizeof(king_t), 1, fp);
  offset += sizeof(king_t);
  // save white king structure
  fseek(fp, offset, SEEK_SET);
  fread(king_white, sizeof(king_t), 1, fp);
  offset += sizeof(king_t);
  // save color (current turn)
  fseek(fp, offset, SEEK_SET);
  fread(current_color, sizeof(color_t), 1, fp);
  fclose(fp);
  return SUCESS;
}

error_chss_t init_brd(cell_t * brd){
  king_black.king_state = SAFE;
  king_black.indx = 4;
  king_white.king_state = SAFE;
  king_white.indx = 60;
  create(&jql_black);
  create(&jql_white);
  create(&history);
  // PAWNS
  for (size_t i = 0; i < 64; i++) {
    brd->p = malloc(sizeof(piece_t));
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
  current_color = WHITE;
  return SUCCESS;
}

error_chss_t get_index(int ch, int num){
  if ((ch < 97) || (ch > 104)) {
    exit(FAILURE);
  }
  if ((num < 49) || (num > 56)) {
    exit(FAILURE);
  }
  ch -= 97;
  num -= 49;
  return (8 * (7 - num)) + ch;
}

char* get_coord(int i){
  if ((i < 0) || (i > 63)) {
    exit(FAILURE);
  }
  char *nd = malloc(sizeof(char)*2);
  nd[0] = (char) 97 + i % 8;
  nd[1] = (char) 56 - i / 8;
  return nd;
}

bool is_valid_index(int i){
  if ((i < 0) || (i > 63)) return false;
  return true;
}

bool is_empty(cell_t * brd, int i){
  if (!is_valid_index(i)) return false;
  return brd[i].p == NULL;
}

bool is_enemy(cell_t * brd, int i, color_t cl){
  if (!is_valid_index(i)) return false;
  if (!is_empty(brd, i)) return brd[i].p->cl != cl;
  return true;
}

const char* get_type(cell_t* brd, int p){
  if ((p < 0) || (p > 63)) return "";
  if (is_empty(brd, p)) return "";
  return type_ch[brd[p].p->tp];
}

const char* get_color(cell_t* brd, int p){
  if ((p < 0) || (p > 63)) return "";
  if (is_empty(brd, p)) return "";
  return color_ch[brd[p].p->cl];
}

// ONLY FOR PAWNS
int _get_next(cell_t * brd, color_t cl, int i, int stps){
  if (cl == WHITE)
    return i - stps;
  else
    return i + stps;
}

bool _same_row(int d, int k){
  if (!is_valid_index(d) || !is_valid_index(k)) return 0;
  return d/8 == k/8;
}

bool _same_column(int d, int k){
  if (!is_valid_index(d) || !is_valid_index(k)) return 0;
  return d%8 == k%8;
}

error_chss_t _check_if_(cell_t * brd, int j, color_t cl, const char* pieces, bool* f){
  *f = false;
  if (is_empty(brd,j)) return FAILURE;
  while (*pieces != '\0') {
    piece_t pc;
    switch (*pieces) {
      case 'p': pc = PAWN;
      case 'h': pc = HORSE;
      case 'b': pc = BISHOP;
      case 't': pc = TOWER;
      case 'q': pc = QUEEN;
      case 'k': pc = KING;
    }
    if ((brd[i].p->cl == cl) && (brd[i].p->tp == pc)) *f = true;
    ++pieces;
  }
  return SUCCESS;
}

error_chss_t get_movlist_pawns(cell_t * brd, int i, int* l, int* sz){
  if (!is_valid_index(i)) return FAILURE;
  if (brd == NULL) return FAILURE;

  int k = _get_next(brd, brd[i].p->cl, i, 8);
  if (!is_valid_index(k))
    return FAILURE;
  else {
    if (is_empty(brd, k)) {
      *l++ = k;
      *sz += 1;
      if ((i / 8 == 1) || (i / 8 == 6)) {
        int j = _get_next(brd,brd[i].p->cl,i,16);
        if (is_valid_index(j))
          if (is_empty(brd, j))
            *l++ = j;
            *sz += 1;
      }
    }
    if (_same_row(k,k-1)) {
      if (is_enemy(brd, k-1, brd[i].p->cl)) {
        *l++ = k-1;
        *sz += 1;
      }
    }
    if (_same_row(k,k+1)) {
      if (is_enemy(brd, k+1, brd[i].p->cl)) {
        *l++ = k+1;
        *sz += 1;
      }
    }
  }
  return SUCCESS;
}

error_chss_t reverse_lookup_pawn(cell_t * brd, int i, color_t l, bool* flag){
  if (!is_valid_index(i)) return FAILURE;
  if (brd == NULL) return FAILURE;
  int k = _get_next(brd, cl, i, -8);
  if (!is_valid_index(k))
    return FAILURE;
  else {
    _check_if_(brd, cl, k, 'p', flag);
    if (!flag && is_empty(brd, k)){
      if ((i / 8 == 3) || (i / 8 == 4)) {
        k = _get_next(brd, cl, i, -16);
        _check_if_(brd, cl, k, 'p', flag);
      }
    }
  }
  return SUCCESS;
}

bool _check_cell(cell_t * brd, int j, int* l, int* sz){
  if (is_empty(brd,j)) {*l++ = j; (*sz)++; return false;}
  if (is_enemy(brd,j,brd[i].p->cl)) {*l++ = j; (*sz)++;}
  return true;
}

error_chss_t _tower_moves(cell_t * brd, int i, const char * fmt, ...){
  va_list args;
  va_start(args, fmt);
  switch (*fmt) {
    case 'l': { // list
      int* l = va_arg(args, int*);
      int* sz = va_arg(args, int*);
    }
    case 'r':{
      color_t color = va_arg(args, color_t);
      bool* flag = va_arg(args, bool*);
    }
    default:
      return FAILURE;
  }
  // going right
  if(_same_row(i, i+1)){
    for (int j = i + 1; j%8 != 0; j++) {
      switch (*fmt) {
        case 'l': if (_check_cell(brd, j, l, sz)) break;
        case 'r': if (_check_if_(brd, j, color, 'qt', flag)) return SUCESS;
      }
    }
  }
  // going left
  if(_same_row(i, i-1)){
    for (int j = i-1; j%8 != 7; j--) {
      switch (*fmt) {
        case 'l': if (_check_cell(brd, j, l, sz)) break;
        case 'r': if (_check_if_(brd, j, color, 'qt', flag)) return SUCESS;
      }
    }
  }
  // going down
  if(_same_column(i, i+8)){
    for (int j = i + 8; j < 64; j+=8) {
      switch (*fmt) {
        case 'l': if (_check_cell(brd, j, l, sz)) break;
        case 'r': if (_check_if_(brd, j, color, 'qt', flag)) return SUCESS;
      }
    }
  }
  // going up
  if(_same_column(i, i-8)){
    for (int j = i-8; j > -1; j-=8) {
      switch (*fmt) {
        case 'l': if (_check_cell(brd, j, l, sz)) break;
        case 'r': if (_check_if_(brd, j, color, 'qt', flag)) return SUCESS;
      }
    }
  }
  va_end(args);
  return SUCCESS;
}

error_chss_t get_movlist_towers(cell_t * brd, int i, int* l, int* sz){
  if (!is_valid_index(i)) return FAILURE;
  if (brd == NULL) return FAILURE;
  // going right
  _tower_moves(brd, i, 'l', l, sz);
  return SUCCESS;
}

error_chss_t reverse_lookup_towers_and_queen(cell_t * brd, int i, color_t cl, bool* flag){
  if (!is_valid_index(i)) return FAILURE;
  if (brd == NULL) return FAILURE;
  // going right
  *flag = false;
  _tower_moves(brd, i, 'r', cl, flag);
  return SUCCESS;
}

error_chss_t _horse_moves(cell_t * brd, int i, int k, int d, const char* fmt, ...){
  int o;
  va_list args;
  va_start(args, fmt);
  switch (*fmt) {
    case 'l': { // list
      int* l = va_arg(args, int*);
      int* sz = va_arg(args, int*);
    }
    case 'r': { // reverse
      color_t color = va_arg(args, color_t);
      bool* flag = va_arg(args, bool*);
    }
    default: return FAILURE;
  }
  if (_same_row(i, k)) {
    if (_same_column(k, o = k+d)) {
        switch (*fmt) {
          case 'l': if (is_empty(brd, o) || is_enemy(brd,o,brd[i].p->cl)){ *l++ = m; (*sz)++; }
          case 'r': _check_if_(brd, j, color, 'h', flag);
        }
      }
    }
  }
  va_end(args);
  return SUCCESS;
}

error_chss_t get_movlist_horses(cell_t * brd, int i, int* l, int* sz){
  if (!is_valid_index(i)) return FAILURE;
  if (brd == NULL) return FAILURE;
  _horse_moves(brd, i, i+1, 16, 'l', l, sz);
  l += (*sz); //IMPORTANT - SHIFTS MEMORY POINTER
  _horse_moves(brd, i, i+2,  8, 'l', l, sz);
  l += (*sz); //IMPORTANT - SHIFTS MEMORY POINTER
  _horse_moves(brd, i, i+1,-16, 'l', l, sz);
  l += (*sz); //IMPORTANT - SHIFTS MEMORY POINTER
  _horse_moves(brd, i, i+2, -8, 'l', l, sz);
  l += (*sz); //IMPORTANT - SHIFTS MEMORY POINTER
  _horse_moves(brd, i, i-1, 16, 'l', l, sz);
  l += (*sz); //IMPORTANT - SHIFTS MEMORY POINTER
  _horse_moves(brd, i, i-2,  8, 'l', l, sz);
  l += (*sz); //IMPORTANT - SHIFTS MEMORY POINTER
  _horse_moves(brd, i, i-1,-16, 'l', l, sz);
  l += (*sz); //IMPORTANT - SHIFTS MEMORY POINTER
  _horse_moves(brd, i, i-2, -8, 'l', l, sz);
  return SUCCESS;
}

error_chss_t reverse_lookup_horses(cell_t * brd, int i, color_t cl, bool* flag){
  if (!is_valid_index(i)) return FAILURE;
  if (brd == NULL) return FAILURE;
  int arr[2] = {i+1, i-1};
  for (size_t i = 0; i < len(arr); i++) {
    _horse_moves(brd, i, arr[i], 16, 'r', cl, flag);
    if (flag) return SUCCESS;
    _horse_moves(brd, i, arr[i], -16, 'r', cl, flag);
    if (flag) return SUCCESS;
  }
  arr[2] = {i+2, i-2};
  for (size_t i = 0; i < len(arr); i++) {
    _horse_moves(brd, i, arr[i], 8, 'r', cl, flag);
    if (flag) return SUCCESS;
    _horse_moves(brd, i, arr[i], -8, 'r', cl, flag);
    if (flag) return SUCCESS;
  }
  return SUCCESS;
}

error_chss_t _bishop_moves(cell_t * brd, int p, const char* fmt, ...){
  va_list args;
  va_start(args, fmt);
  switch (*fmt) {
    case 'l': { // list
      int* l = va_arg(args, int*);
      int* sz = va_arg(args, int*);
    }
    case 'r':{
      color_t color = va_arg(args, color_t);
      bool* flag = va_arg(args, bool*);
    }
    default:
      return FAILURE;
  }
  // going right up
  for (size_t j = p - 7; j%8 != 0; j -= 7){
    switch (*fmt) {
      case 'l': if (_check_cell(brd, j, l, sz)) break;
      case 'r': if (_check_if_(brd, j, color, 'bq', flag)) return SUCESS;
    }
  }
  // going right down
  for (size_t j = p + 9; j%8 != 0; j += 9) {
    switch (*fmt) {
      case 'l': if (_check_cell(brd, j, l, sz)) break;
      case 'r': if (_check_if_(brd, j, color, 'bq', flag)) return SUCESS;
    }
  }
  // going left down
  for (size_t j = p + 7; j%8 != 7; j += 7) {
    switch (*fmt) {
      case 'l': if (_check_cell(brd, j, l, sz)) break;
      case 'r': if (_check_if_(brd, j, color, 'bq', flag)) return SUCESS;
    }
  }
  // going left up
  for (size_t j = p - 9; j%8 != 7; j -= 9){
    switch (*fmt) {
      case 'l': if (_check_cell(brd, j, l, sz)) break;
      case 'r': if (_check_if_(brd, j, color, 'bq', flag)) return SUCESS;
    }
  }
  return SUCCESS;
}

error_chss_t get_movlist_bishops(cell_t * brd, int p, int* l, int* sz){
  if (!is_valid_index(p)) return FAILURE;
  if (brd == NULL) return FAILURE;
  _bishop_moves(brd, p, 'l', l, sz);
  return SUCCESS;
}

error_chss_t reverse_lookup_bishops_and_queen(cell_t* brd, int p, color_t cl, bool* flag){
  if (!is_valid_index(p)) return FAILURE;
  if (brd == NULL) return FAILURE;
  _bishop_moves(brd, p, 'r', cl, flag);
  return SUCCESS;
}

error_chss_t convert_index_4_king(int p, int i, int* k){
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

error_chss_t get_movlist_kings(cell_t * brd, int p, int* l, int* sz){
  if (!is_valid_index(p)) return FAILURE;
  if (brd == NULL) return FAILURE;
  int k; list_t jql; int answer;
  if (brd[p].p->cl == WHITE) jql = jql_white;
  else jql = jql_black;
  for (int i = 0; i < 9; i++) {
    if (convert_index_4_king(p, i, &k)){
      if (k != -1) {
        if (!in(&jql, k))
          if (is_empty(brd,k) || is_enemy(brd,k,brd[k].p->cl)) {*l++ = k; *sz += 1;}
      }
    }
  }
  return SUCCESS;
}

error_chss_t get_movlist(cell_t * brd, int p, int* l, int* sz){
  if (brd == NULL) return FAILURE;
  if (!is_valid_index(p)) return FAILURE;
  if (is_empty(brd,p)) return NOPIECE;
  *sz = 0;                            // Size of the list_t
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
      if (!get_movlist_towers(brd, p, l, sz)) return FAILURE;
      l += *sz; //IMPORTANT - SHIFTS MEMORY POINTER
      if (!get_movlist_bishops(brd, p, l, sz)) return FAILURE;
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

error_chss_t _get_estate(cell_t* brd, int j, int king, type_piece_t t1, type_piece_t t2){
  if (!is_valid_index(j)) return FAILURE;
  if (is_empty(brd, j))  return NOPIECE;
  if (((brd[j].p->tp == t1) || (brd[j].p->tp == t2))  && (is_enemy(brd, j, brd[king].p->cl))) return SUCCESS;
  return FAILURE;
}

bool king_in_check(cell_t* brd, int p, int king){
  // check same _same_row
  if (_same_row(king, p)){
    // going right
    for (int j = king + 1; j%8 != 0; j++){
      switch (_get_estate(brd, j, king, TOWER, QUEEN)) {
        case FAILURE: break;
        case SUCCESS: return true;
        case NOPIECE: continue;
      }
    }
    // going left
    for (int j = king - 1; j%8 != 7; j--){
      switch (_get_estate(brd, j, king, TOWER, QUEEN)) {
        case FAILURE: break;
        case SUCCESS: return true;
        case NOPIECE: continue;
      }
    }
  }
  // check same _same_column
  if (_same_column(king, p)){
      // going down
      for (int j = king + 8; j > 64; j+=8){
        switch (_get_estate(brd, j, king, TOWER, QUEEN)) {
          case FAILURE: break;
          case SUCCESS: return true;
          case NOPIECE: continue;
        }
      }
      // going up
      for (int j = king - 8; j < -1; j-=8){
        switch (_get_estate(brd, j, king, TOWER, QUEEN)) {
          case FAILURE: break;
          case SUCCESS: return true;
          case NOPIECE: continue;
        }
      }
  }
  // going right up
  for (int j = king - 7; j%8 != 0; j -= 7){
    switch (_get_estate(brd, j, king, BISHOP, QUEEN)) {
      case FAILURE: break;
      case SUCCESS: return true;
      case NOPIECE: continue;
    }
  }
  // going right down
  for (int j = king + 9; j%8 != 0; j += 9) {
    switch (_get_estate(brd, j, king, BISHOP, QUEEN)) {
      case FAILURE: break;
      case SUCCESS: return true;
      case NOPIECE: continue;
    }
  }
  // going left down
  for (int j = king + 7; j%8 != 7; j += 7) {
    switch (_get_estate(brd, j, king, BISHOP, QUEEN)) {
      case FAILURE: break;
      case SUCCESS: return true;
      case NOPIECE: continue;
    }
  }
  // going left up
  for (int j = king - 9; j%8 != 7; j -= 9){
    switch (_get_estate(brd, j, king, BISHOP, QUEEN)) {
      case FAILURE: break;
      case SUCCESS: return true;
      case NOPIECE: continue;
    }
  }
  return false;
}

error_chss_t move_piece(cell_t * brd, int p, int q, color_t cl_player){
  list_t* jql;
  king_t* king_st;
  if (current_color != cl_player) return FAILURE; // Replace with enum error type
  if (brd == NULL) return FAILURE;
  if (!is_valid_index(p)) return FAILURE;
  if (!is_valid_index(q)) return FAILURE;
  if (is_empty(brd, p)) return NOPIECE;
  if (brd[p].p->cl != cl_player) return FAILURE;
  if (cl_player == BLACK){
    jql = &jql_black;
    king_st = &king_black;
  } else {
    jql = &jql_white;
    king_st = &king_white;
  }
  if (king_st->king_state > 0) return FAILURE; // King in check
  int szq = 0; int szp = 0;
  int* valid_movs_p = malloc(sizeof(int)*30);
  if (valid_movs_p == NULL) return FAILURE;
  int* valid_movs_q = malloc(sizeof(int)*30);
  if (valid_movs_q == NULL) return FAILURE;
  get_movlist(brd, p, valid_movs_p, &szp);
  if (szp == 0) return FAILURE;
  // Check if q is a valid index to move in
  for (size_t i = 0; i < szp; i++) {
    if (q == valid_movs_p[i]) break;
    if (i == szp-1) return FAILURE;
  }
  // Swap cells
  brd[q].p = malloc(sizeof(piece_t));
  if (brd[q].p == NULL) return FAILURE;
  brd[q].p->tp = brd[p].p->tp;
  brd[q].p->cl = brd[p].p->cl;
  remove_list(jql, valid_movs_p, szp);
  get_movlist(brd, q, valid_movs_q, &szq);
  append_unique_list(jql, valid_movs_q, szq);
  // Change king stored position
  if (brd[q].p->tp == KING) {
    king_st->indx = q;
  }
  /* Add indexes (previous and actual) to list.
     ***EXPLANATION***:
     HERE we ADD p and q in the same structure,
     now we don't need a counter so we manipulate
     the list structure to save space.
  */
  append(&history, p, q);
  // ONLY if piece index p was on same line, same row or same diagonals of the King
  // OR if king index is in the jql list
  else if ((in(jql, king_st->indx)) || (king_in_check(brd, p, king_st->indx))) {
    king_st->king_state = JAQUE;
    int szk = 0;
    int* lk = malloc(sizeof(int)*30);
    if (valid_movs_p == NULL) return FAILURE;
    get_movlist(brd, king_st->indx, lk, &szk);
    // TODO - Check when the king is in jaque_mate
    if (szk == 0)
      if (!blockpossible(brd)) jaque_mate = 1;
  }
  brd[q].p = NULL;
  if (current_color == WHITE) current_color = BLACK;
  else current_color = WHITE;
  return SUCCESS;
}

#endif
