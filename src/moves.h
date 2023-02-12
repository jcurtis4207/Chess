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

extern char en_passant_target_square[2];
extern char en_passant_victim_square[2];
extern int en_passant_time;

int is_same_color(char *board, char cur_col, char cur_row, char new_col, char new_row);
int is_column_clear(char *board, char cur_col, char cur_row, char new_col, char new_row);
int is_row_clear(char *board, char cur_col, char cur_row, char new_col, char new_row);
int is_diagonal_clear(char *board, char cur_col, char cur_row, char new_col, char new_row);

int linear_move(char *board, char cur_col, char cur_row, char new_col, char new_row);
int diagonal_move(char *board, char cur_col, char cur_row, char new_col, char new_row);

void enable_en_passant(char cur_col, char cur_row, char new_col, char new_row);
int perform_en_passant(char *board, char new_col, char new_row);
void promote_pawn(char *board, char col, char row, int turn);

int check_pawn_direction(int vertical_distance, int turn);
int check_pawn_distance(char cur_col, char cur_row, char new_col, char new_row, int turn);
int check_pawn_taking(char *board, char cur_col, char cur_row, char new_col, char new_row);

int rook_move(char *board, char cur_col, char cur_row, char new_col, char new_row);
int bishop_move(char *board, char cur_col, char cur_row, char new_col, char new_row);
int queen_move(char *board, char cur_col, char cur_row, char new_col, char new_row);
int king_move(char *board, char cur_col, char cur_row, char new_col, char new_row);
int pawn_move(char *board, char cur_col, char cur_row, char new_col, char new_row, int turn);
int knight_move(char *board, char cur_col, char cur_row, char new_col, char new_row);

int is_square_off_board(char cur_col, char cur_row, char new_col, char new_row);
int is_current_square_empty(char *board, char cur_col, char cur_row);
int is_current_square_opponent_piece(char *board, char cur_col, char cur_row, int turn);

int is_legal_move(char *board, char cur_col, char cur_row, char new_col, char new_row, int turn);
int move(char *board, char cur_col, char cur_row, char new_col, char new_row, int turn);
