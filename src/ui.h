#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include "board.h"
#include "moves.h"

#define PIECE_SIZE 24
#define BOX_SIZE 59

#define SQUARE_WIDTH 9
#define SQUARE_HEIGHT 6
#define X_OFFSET 5

enum colors {
    BLACK_PIECE = 1,
    WHITE_PIECE = 2,
    NORMAL = 3,
    DARK = 4
};

int init_ui();
int destroy_ui();

void draw_board(char *board);
void draw_prompt(int turn);

void read_input(char *cur_col, char *cur_row, char *new_col, char *new_row);
void get_pawn_promotion_input(char *new_piece, int turn);
void print_error(char *error);
