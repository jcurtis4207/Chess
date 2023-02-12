#include "board.h"

char get_piece_at_position(char *board, char col, char row) {
    int col_num = col - 'a';
    int row_num = row - '1';
    return board[row_num * NUM_ROWS + col_num];
}

void set_piece_at_position(char *board, char col, char row, char piece) {
    int col_num = col - 'a';
    int row_num = row - '1';
    board[row_num * NUM_ROWS + col_num] = piece;
}

int get_type_at_position(char *board, char col, char row) {
    char piece = get_piece_at_position(board, col, row);
    switch(piece) {
        case 'r':
        case 'R':
            return TYPE_ROOK;
            break;
        case 'n':
        case 'N':
            return TYPE_KNIGHT;
            break;
        case 'b':
        case 'B':
            return TYPE_BISHOP;
            break;
        case 'q':
        case 'Q':
            return TYPE_QUEEN;
            break;
        case 'k':
        case 'K':
            return TYPE_KING;
            break;
        case 'p':
        case 'P':
            return TYPE_PAWN;
            break;
        default:
            return TYPE_EMPTY;
            break;
    }
}
