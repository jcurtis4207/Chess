#include <stdio.h>
#include "board.h"
#include "moves.h"
#include "ui.h"

void perform_pawn_promotion(char *board, char col, char row, int *turn) {
    draw_board(board);
    char new_piece;
    get_pawn_promotion_input(&new_piece, *turn);
    set_piece_at_position(board, col, row, new_piece);
    *turn = (*turn + 1) % 2;
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

    init_ui();

    char cur_col, cur_row, new_col, new_row;
    int turn = WHITE_MOVE;

    do {
        draw_board(board);
        draw_prompt(turn);
        read_input(&cur_col, &cur_row, &new_col, &new_row);
        int result = move_piece(board, tolower(cur_col), cur_row, tolower(new_col), new_row, turn);
        if (result == true) {
            turn = (turn + 1) % 2;
        } else if (result == PAWN_PROMOTION) {
            perform_pawn_promotion(board, new_col, new_row, &turn);
        } else {
            print_error(error_message);
        }
    } while (true);

    destroy_ui();
    return 0;
}
