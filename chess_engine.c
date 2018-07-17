#include <studio.h>
#include <stdint.h>

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
typedef struct type_piece_t type_piece;

enum color_t{
  BLACK,
  WHITE
};
typedef enum color_t color;

struct piece_t{
  type_piece tp;
  color cl;
};
typedef struct piece_t piece;

struct node_t{
  uint8_t ch;  // char
  uint8_t num;  // number
};
typedef struct node_t node;

struct cell_t{
  node *nd;
  piece *p;
};
typedef struct cell_t cell;

// ERROR HANDLING
enum error_t{
  SUCCESS,
  FAILURE,
  NO_PIECE
};
typedef enum error_t error;

int init_board(board * brd){
  uint8_t l = 65;
  for (size_t i = 0; i < 8; i++) {
    brd[i*8].nd->ch = l;
    uint8_t m = 38;
    for (size_t j = 0; j < 8; j++) {
      brd[i+j].nd->num = m;
      m -= j;
    }
    l += i;
  }
  return SUCCESS;
}

int init_pieces(board * brd){
  // PAWNS
  for (size_t i = 8; i < 16; i++) {
    brd[i].p->tp = PAWN;
    brd[i].p->cl = BLACK;
  }
  for (size_t i = 48; i < 56; i++) {
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
  return SUCCESS;
}

int get_index(uint8_t ch, uint8_t num, int * i){
  if (ch < 65) || (ch > 72) {
    return FAILURE;
  }
  if (ch < 31) || (ch > 38) {
    return FAILURE;
  }
  ch -= 65;
  num -= 31;
  *i = 8 * (8 - num) + ch;
  return SUCCESS;
}

int get_coord(int i, node * nd){
  if (ch < 0) || (ch > 63) {
    return FAILURE;
  }
  nd->ch = i % 8;
  nd->num = i / 8;
  return SUCCESS;
}

int main(int argc, char const *argv[]) {
  cell *board = malloc(sizeof(cell)*BRD_LEN);
  if (board == NULL){
    exit(EXIT_FAILURE);
  }
  if (!init_board(&board)){
    exit(EXIT_FAILURE);
  }
  if (!init_pieces(&board)){
    exit(EXIT_FAILURE);
  }
  // play chess
  free(board);
  return 0;
}
