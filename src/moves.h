#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "board.h"

#define true 1
#define false 0

enum turn {
    WHITE_MOVE = 0,
    BLACK_MOVE = 1
};

int is_same_color(char *board, char cur_col, char cur_row, char new_col, char new_row);
int is_column_clear(char *board, char cur_col, char cur_row, char new_col, char new_row);
int is_row_clear(char *board, char cur_col, char cur_row, char new_col, char new_row);
int is_diagonal_clear(char *board, char cur_col, char cur_row, char new_col, char new_row);

int linear_move(char *board, char cur_col, char cur_row, char new_col, char new_row);
int diagonal_move(char *board, char cur_col, char cur_row, char new_col, char new_row);

int rook_move(char *board, char cur_col, char cur_row, char new_col, char new_row);
int bishop_move(char *board, char cur_col, char cur_row, char new_col, char new_row);
int queen_move(char *board, char cur_col, char cur_row, char new_col, char new_row);
int king_move(char *board, char cur_col, char cur_row, char new_col, char new_row);
int pawn_move(char *board, char cur_col, char cur_row, char new_col, char new_row, int turn);
int knight_move(char *board, char cur_col, char cur_row, char new_col, char new_row);

int is_legal_move(char *board, char cur_col, char cur_row, char new_col, char new_row, int turn);
int move(char *board, char cur_col, char cur_row, char new_col, char new_row, int turn);
