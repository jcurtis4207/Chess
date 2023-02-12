#include <stdio.h>
#include "board.h"
#include "moves.h"

void print_white_piece(char piece) {
    printf("\033[30;107m%c \033[0m", piece);
}

void print_black_piece(char piece) {
    printf("\033[97;40m%c \033[0m", toupper(piece));
}

void print_board(char *board) {
    printf("\n");
    int row_label = 8;
    for (int row = NUM_ROWS - 1; row >= 0; row--) {
        printf("%d ", row_label--);
        for (int col = 0; col < NUM_ROWS; col++) {
            char piece = board[row * NUM_ROWS + col];
            if (piece == 0) {
                printf("- ");
            } else if (isupper(piece)) {
                print_white_piece(piece);
            } else {
                print_black_piece(piece);
            }
        }
        printf("\n");
    }
    printf("  ");
    for (char c = 'a'; c <= 'h'; c++) {
        printf("%c ", c);
    }
    printf("\n\n");
}

int main() {
    char chess_board[8][8] = {
        {'R','N','B','Q','K','B','N','R'},
        {'P','P','P','P','P','P','P','P'},
        { 0,  0,  0,  0,  0,  0,  0,  0 },
        { 0,  0,  0,  0,  0,  0,  0,  0 },
        { 0,  0,  0,  0,  0,  0,  0,  0 },
        { 0,  0,  0,  0,  0,  0,  0,  0 },
        {'p','p','p','p','p','p','p','p'},
        {'r','n','b','q','k','b','n','r'}
    };
    char *board = &chess_board[0][0];

    char cur_col, cur_row, new_col, new_row;
    int turn = WHITE_MOVE;
    do {
        print_board(board);

        printf("Next Move (%s): ", (turn == WHITE_MOVE) ? "White" : "Black");
        scanf(" %c%c %c%c", &cur_col, &cur_row, &new_col, &new_row);

        if (move(board, tolower(cur_col), cur_row, tolower(new_col), new_row, turn)) {
            turn = (turn + 1) % 2;
        }
    } while (true);

    return 0;
}
