#pragma once
#include <stdio.h>

#define NUM_ROWS 8

enum piece_type {
    TYPE_EMPTY = 0,
    TYPE_PAWN = 1,
    TYPE_ROOK = 2,
    TYPE_KNIGHT = 3,
    TYPE_BISHOP = 4,
    TYPE_QUEEN = 5,
    TYPE_KING = 6
};

char get_piece_at_position(char *board, char col, char row);
void set_piece_at_position(char *board, char col, char row, char piece);
int get_type_at_position(char *board, char col, char row);
