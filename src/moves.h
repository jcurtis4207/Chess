#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "board.h"

#define true 1
#define false 0
#define PAWN_PROMOTION 127

enum turn {
    WHITE_MOVE = 0,
    BLACK_MOVE = 1
};

extern char en_passant_target_square[2];
extern char en_passant_victim_square[2];
extern int en_passant_time;

extern char error_message[100];

int is_legal_move(char *board, char cur_col, char cur_row, char new_col, char new_row, int turn);
int move_piece(char *board, char cur_col, char cur_row, char new_col, char new_row, int turn);
