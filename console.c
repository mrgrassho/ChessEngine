#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"
#include "chess_engine.h"

void print_all(cell_t * board, int* l, int p, int sz){
  printf("%2s(%i) [%s %s]\n",  get_coord(p), p, get_type(board, p), get_color(board, p));
  for (size_t i = 0; i < sz; i++) {
    printf("%2s(%i)", get_coord(l[i]), l[i]);
    if (i != sz-1) printf(" - ");
  }
  if (sz > 0) printf("\n");
}

int main(int argc, char const *argv[]) {
  //DEBUG: 2 -> Rey
  cell_t* board = malloc(sizeof(cell_t)*BRD_LEN);
  if (board == NULL) return FAILURE;
  if (!init_brd(board)) return FAILURE;
  int flag = 1;
  color_t ccolor = WHITE;
  while (flag) {
    printf("> ");
    char cmd;
    scanf(" %c", &cmd);
    switch (cmd) {
      case 'm': { //move
        char args[5];
        scanf(" %s", args);
        if (move_piece(board, get_index(args[0], args[1]), get_index(args[3], args[4]), ccolor)){
          if (ccolor == WHITE) ccolor = BLACK;
          else ccolor = WHITE;
        }
        break;
      }
      case 'v': { //view
        char args[2];
        int sz = 0;
        int* l = malloc(sizeof(int)*64);
        scanf(" %s", args);
        if (get_movlist(board, get_index(args[0], args[1]), l, &sz))
          print_all(board, l, get_index(args[0], args[1]), sz);
        free(l);
        break;
      }
      default: { //quit
        flag = 0;
        break;
      }
    }
  }
  return 0;
}
