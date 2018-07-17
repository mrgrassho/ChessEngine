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

struct cell_t{
  uint8_t ch;  // char
  uint8_t num;  // number
  piece *p;
};
typedef struct cell_t cell;

int init_board(board * brd){
  uint8_t l = 65;
  for (size_t i = 0; i < 8; i++) {
    brd[i*8]brd->ch = l;
    uint8_t m = 38;
    for (size_t j = 0; j < 8; j++) {
      brd[i+j]->num = m;
      m -= j;
    }
    l += i;
  }
  return 1;
}

int init_pawns(board * brd){
  for (size_t i = 8; i < 16; i++) {
    brd[i]->p.tp = type_piece.PAWN;
    brd[i]->p.cl = color.BLACK;
  }
  for (size_t i = 48; i < 56; i++) {
    brd[i]->p.tp = type_piece.PAWN;
    brd[i]->p.cl = color.WHITE;
  }
  return 1;
}

int init_horses(board * brd){
  brd[1]->p.tp = type_piece.HORSE;
  brd[1]->p.cl = color.BLACK;
  brd[6]->p.tp = type_piece.HORSE;
  brd[6]->p.cl = color.BLACK;
  brd[57]->p.tp = type_piece.HORSE;
  brd[57]->p.cl = color.WHITE;
  brd[62]->p.tp = type_piece.HORSE;
  brd[62]->p.cl = color.WHITE;
  return 1;
}

int init_towers(board * brd){
  brd[0]->p.tp = type_piece.TOWER;
  brd[0]->p.cl = color.BLACK;
  brd[7]->p.tp = type_piece.TOWER;
  brd[7]->p.cl = color.BLACK;
  brd[56]->p.tp = type_piece.TOWER;
  brd[56]->p.cl = color.WHITE;
  brd[63]->p.tp = type_piece.TOWER;
  brd[63]->p.cl = color.WHITE;
  return 1;
}

int init_bishops(board * brd){
  brd[2]->p.tp = type_piece.BISHOP;
  brd[2]->p.cl = color.BLACK;
  brd[5]->p.tp = type_piece.BISHOP;
  brd[5]->p.cl = color.BLACK;
  brd[58]->p.tp = type_piece.BISHOP;
  brd[58]->p.cl = color.WHITE;
  brd[61]->p.tp = type_piece.BISHOP;
  brd[61]->p.cl = color.WHITE;
  return 1;
}

int init_queens(board * brd){
  brd[3]->p.tp = type_piece.QUEEN;
  brd[3]->p.cl = color.BLACK;
  brd[59]->p.tp = type_piece.QUEEN;
  brd[59]->p.cl = color.WHITE;
  return 1;
}

int init_kings(board * brd){
  brd[4]->p.tp = type_piece.KING;
  brd[4]->p.cl = color.BLACK;
  brd[60]->p.tp = type_piece.KING;
  brd[60]->p.cl = color.WHITE;
  return 1;
}

int init_pieces(board * brd){
  if !init_pawns(*board) exit(EXIT_FAILURE);
  if !init_horses(*board) exit(EXIT_FAILURE);
  if !init_bishops(*board) exit(EXIT_FAILURE);
  if !init_towers(*board) exit(EXIT_FAILURE);
  if !init_queens(*board) exit(EXIT_FAILURE);
  if !init_kings(*board) exit(EXIT_FAILURE);
  return 1;
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
