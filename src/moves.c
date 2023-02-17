#include "moves.h"

char en_passant_target_square[2] = {0,0};
char en_passant_victim_square[2] = {0,0};
int en_passant_time = 0;

char error_message[100];

char white_can_castle_kingside = true;
char white_can_castle_queenside = true;
char black_can_castle_kingside = true;
char black_can_castle_queenside = true;

int is_same_color(char *board, char cur_col, char cur_row, char new_col, char new_row) {
    char current = get_piece_at_position(board, cur_col, cur_row);
    char new = get_piece_at_position(board, new_col, new_row);
    if (new == 0)
        return false;
    return (isupper(current) > 0) ^ (isupper(new) == 0);
}

int is_column_clear(char *board, char cur_col, char cur_row, char new_col, char new_row) {
    int direction = new_row - cur_row;
    cur_row = (direction > 0) ? cur_row + 1 : cur_row - 1;

    while (cur_row != new_row) {
        if (get_piece_at_position(board, cur_col, cur_row) != TYPE_EMPTY) {
            sprintf(error_message, "Illegal Move: column is not clear");
            return false;
        }
        cur_row = (direction > 0) ? cur_row + 1 : cur_row - 1;
    }
    return true;
}

int is_row_clear(char *board, char cur_col, char cur_row, char new_col, char new_row) {
    int direction = new_col - cur_col;
    cur_col = (direction > 0) ? cur_col + 1 : cur_col - 1;

    while (cur_col != new_col) {
        if (get_piece_at_position(board, cur_col, cur_row) != TYPE_EMPTY) {
            sprintf(error_message, "Illegal Move: row is not clear");
            return false;
        }
        cur_col = (direction > 0) ? cur_col + 1 : cur_col - 1;
    }
    return true;
}

int is_diagonal_clear(char *board, char cur_col, char cur_row, char new_col, char new_row) {
    int col_direction = new_col - cur_col;
    int row_direction = new_row - cur_row;
    cur_col = (col_direction > 0) ? cur_col + 1 : cur_col - 1;
    cur_row = (row_direction > 0) ? cur_row + 1 : cur_row - 1;

    while (cur_col != new_col && cur_row != new_row) {
        if (get_piece_at_position(board, cur_col, cur_row) != TYPE_EMPTY) {
            sprintf(error_message, "Illegal Move: diagonal is not clear");
            return false;
        }
        cur_col = (col_direction > 0) ? cur_col + 1 : cur_col - 1;
        cur_row = (row_direction > 0) ? cur_row + 1 : cur_row - 1;
    }
    return true;
}

int linear_move(char *board, char cur_col, char cur_row, char new_col, char new_row) {
    int vertical_distance = new_row - cur_row;
    int horizontal_distance = new_col - cur_col;
    if (vertical_distance != 0 && horizontal_distance != 0) {
        sprintf(error_message, "Illegal Move: move is not linear");
        return false;
    }
    if (vertical_distance != 0) {
        return is_column_clear(board, cur_col, cur_row, new_col, new_row);
    } else {
        return is_row_clear(board, cur_col, cur_row, new_col, new_row);
    }
}

int diagonal_move(char *board, char cur_col, char cur_row, char new_col, char new_row) {
    int vertical_distance = new_row - cur_row;
    int horizontal_distance = new_col - cur_col;
    if (abs(vertical_distance) != abs(horizontal_distance)) {
        sprintf(error_message, "Illegal Move: move is not diagonal");
        return false;
    }
    return is_diagonal_clear(board, cur_col, cur_row, new_col, new_row);
}

void disable_castle(char col, int turn) {
    // Rook moves disables castling on that side
    if (col == 'a' && turn == WHITE_MOVE) {
        white_can_castle_queenside = false;
    } else if (col == 'h' && turn == WHITE_MOVE) {
        white_can_castle_kingside = false;
    } else if (col == 'a' && turn == BLACK_MOVE) {
        black_can_castle_queenside = false;
    } else if (col == 'h' && turn == BLACK_MOVE) {
        black_can_castle_kingside = false;
    }
    // King moves disables castling for that color
    else if (turn == WHITE_MOVE) {
        white_can_castle_kingside = false;
        white_can_castle_queenside = false;
    } else if (turn == BLACK_MOVE) {
        black_can_castle_kingside = false;
        black_can_castle_queenside = false;
    }
}

int can_castle(char *board, char cur_col, char cur_row, char new_col, char new_row, int turn) {
    // king and rook must not have moved
    // and no pieces can be between the king and rook
    if (turn == WHITE_MOVE) {
        if (new_col == 'g') {
            if (white_can_castle_kingside && is_row_clear(board, cur_col, cur_row, 'h', '1')) {
                return true;
            } else {
                sprintf(error_message, "Illegal Move: White king cannot castle now");
                return false;
            }
        } else if (new_col == 'c') {
            if (white_can_castle_queenside && is_row_clear(board, cur_col, cur_row, 'a', '1')) {
                return true;
            } else {
                sprintf(error_message, "Illegal Move: White king cannot castle now");
                return false;
            }
        } else {
            sprintf(error_message, "Illegal Move: king cannot more than 1 space");
            return false;
        }
    } else {
        if (new_col == 'g') {
            if (black_can_castle_kingside && is_row_clear(board, cur_col, cur_row, 'h', '8')) {
                return true;
            } else {
                sprintf(error_message, "Illegal Move: Black king cannot castle now");
                return false;
            }
        } else if (new_col == 'c') {
            if (black_can_castle_queenside && is_row_clear(board, cur_col, cur_row, 'a', '8')) {
                return true;
            } else {
                sprintf(error_message, "Illegal Move: Black king cannot castle now");
                return false;
            }
        } else {
            sprintf(error_message, "Illegal Move: king cannot more than 1 space");
            return false;
        }
    }
}

void castle_move_rook(char *board, char king_new_col, int turn) {
    // white queenside
    if (turn == WHITE_MOVE && king_new_col == 'c') {
        set_piece_at_position(board, 'd', '1', 'R');
        set_piece_at_position(board, 'a', '1', 0);
    }
    // white kingside
    else if (turn == WHITE_MOVE && king_new_col == 'g') {
        set_piece_at_position(board, 'f', '1', 'R');
        set_piece_at_position(board, 'h', '1', 0);
    }
    // black queenside
    else if (turn == BLACK_MOVE && king_new_col == 'c') {
        set_piece_at_position(board, 'd', '8', 'r');
        set_piece_at_position(board, 'a', '8', 0);
    }
    // black kingside
    else if (turn == BLACK_MOVE && king_new_col == 'g') {
        set_piece_at_position(board, 'f', '8', 'r');
        set_piece_at_position(board, 'h', '8', 0);
    } else {
        sprintf(error_message, "Error castling rook");
    }

}

int rook_move(char *board, char cur_col, char cur_row, char new_col, char new_row, int turn) {
    disable_castle(cur_col, turn);
    return linear_move(board, cur_col, cur_row, new_col, new_row);
}

int bishop_move(char *board, char cur_col, char cur_row, char new_col, char new_row) {
    return diagonal_move(board, cur_col, cur_row, new_col, new_row);
}

int queen_move(char *board, char cur_col, char cur_row, char new_col, char new_row) {
    if (diagonal_move(board, cur_col, cur_row, new_col, new_row)) {
        return true;
    } else {
        return linear_move(board, cur_col, cur_row, new_col, new_row);
    }
}

int king_move(char *board, char cur_col, char cur_row, char new_col, char new_row, int turn) {
    int vertical_distance = new_row - cur_row;
    int horizontal_distance = new_col - cur_col;
    if (abs(vertical_distance) > 1) {
        sprintf(error_message, "Illegal Move: king cannot more than 1 space");
        return false;
    } else if (abs(horizontal_distance) > 1) {
        if (can_castle(board, cur_col, cur_row, new_col, new_row, turn)) {
            castle_move_rook(board, new_col, turn);
            disable_castle(0, turn);
            return true;
        } else {
            return false;
        }
    }
    if (diagonal_move(board, cur_col, cur_row, new_col, new_row) ||
        linear_move(board, cur_col, cur_row, new_col, new_row)) {
            disable_castle(0, turn);
            return true;
    } else {
        return false;
    }
}

int check_pawn_direction(int vertical_distance, int turn) {
    if (vertical_distance > 0 && turn == BLACK_MOVE) {
        sprintf(error_message, "Illegal Move: pawn cannot move backwards");
        return false;
    } else if (vertical_distance < 0 && turn == WHITE_MOVE) {
        sprintf(error_message, "Illegal Move: pawn cannot move backwards");
        return false;
    } else {
        return true;
    }
}

void enable_en_passant(char cur_col, char cur_row, char new_col, char new_row) {
    // set en_passant_square to square that was jumped
    // this is the target square for an attacking pawn on the next turn
    char skipped_row = (new_row - cur_row) / 2 + cur_row;
    en_passant_target_square[0] = cur_col;
    en_passant_target_square[1] = skipped_row;
    // this is the square of the piece that will be taken
    en_passant_victim_square[0] = new_col;
    en_passant_victim_square[1] = new_row;
    en_passant_time = 2;
}

int perform_en_passant(char *board, char new_col, char new_row) {
    if (en_passant_time > 0 &&
        en_passant_target_square[0] == new_col &&
        en_passant_target_square[1] == new_row) {
            set_piece_at_position(board, en_passant_victim_square[0], en_passant_victim_square[1], 0);
            return true;
    } else {
        return false;
    }
}

int check_pawn_distance(char cur_col, char cur_row, char new_col, char new_row, int turn) {
    int vertical_distance = new_row - cur_row;
    if (abs(vertical_distance) > 1) {
        // can only move 2 spaces on first move
        if ((turn == WHITE_MOVE && cur_row == '2' && vertical_distance == 2) ||
            (turn == BLACK_MOVE && cur_row == '7' && vertical_distance == -2)) {
                enable_en_passant(cur_col, cur_row, new_col, new_row);
                return true;
        } else {
            sprintf(error_message, "Illegal Move: pawn cannot move more than 1 space after its first move");
            return false;
        }
    } else if (abs(new_col - cur_col) > 1) {
        sprintf(error_message, "Illegal Move: pawn cannot move horizontally");
        return false;
    } else {
        return true;
    }
}

int check_pawn_taking(char *board, char cur_col, char cur_row, char new_col, char new_row) {
    // can only move diagonally when taking opponent's piece
    if (abs(new_col - cur_col) == 1 &&
        get_type_at_position(board, new_col, new_row) != TYPE_EMPTY &&
        !is_same_color(board, cur_col, cur_row, new_col, new_row)) {
            return true;
    } else if (perform_en_passant(board, new_col, new_row)) {
        return true;
    } else {
        sprintf(error_message, "Illegal Move: pawn cannot move horiztonally");
        return false;
    }
}

int pawn_move(char *board, char cur_col, char cur_row, char new_col, char new_row, int turn) {
    if (!check_pawn_direction(new_row - cur_row, turn)) {
        return false;
    }
    if (!check_pawn_distance(cur_col, cur_row, new_col, new_row, turn)) {
        return false;
    }
    if (abs(new_col - cur_col) > 0) {
        if (check_pawn_taking(board, cur_col, cur_row, new_col, new_row)) {
            return true;
        } else {
            return false;
        }
    }
    if (get_type_at_position(board, new_col, new_row) != TYPE_EMPTY) {
        sprintf(error_message, "Illegal Move: pawn cannot take vertically");
        return false;
    }
    int move = linear_move(board, cur_col, cur_row, new_col, new_row);
    if (!move) {
        return false;
    }
    if ((new_row == '8' && turn == WHITE_MOVE) ||
        (new_row == '1' && turn == BLACK_MOVE)) {
        return PAWN_PROMOTION;
    }
    return true;
}

int knight_move(char *board, char cur_col, char cur_row, char new_col, char new_row) {
    int vertical_distance = abs(new_row - cur_row);
    int horizontal_distance = abs(new_col - cur_col);
    if (vertical_distance == horizontal_distance) {
        sprintf(error_message, "Illegal Move: knight cannot move diagonally");
        return false;
    } else if (vertical_distance == 0 || horizontal_distance == 0) {
        sprintf(error_message, "Illegal Move: knight cannot move linearly");
        return false;
    } else if (vertical_distance + horizontal_distance != 3) {
        sprintf(error_message, "Illegal Move: knight must move 3 spaces");
        return false;
    }
    return true;
}

int is_square_off_board(char cur_col, char cur_row, char new_col, char new_row) {
    if (cur_col < 'a' || cur_col > 'h' || cur_row < '1' || cur_row > '8' ||
        new_col < 'a' || new_col > 'h' || new_row < '1' || new_row > '8') {
        sprintf(error_message, "Illegal Move: position outside of range");
        return true;
    } else {
        return false;
    }
}

int is_current_square_empty(char *board, char cur_col, char cur_row) {
    char current_square = get_piece_at_position(board, cur_col, cur_row);
    if (current_square == TYPE_EMPTY) {
        sprintf(error_message, "Illegal Move: no piece at current position");
        return true;
    } else {
        return false;
    }
}

int is_current_square_opponent_piece(char *board, char cur_col, char cur_row, int turn) {
    char current_piece = get_piece_at_position(board, cur_col, cur_row);
    if (isupper(current_piece) && turn == BLACK_MOVE) {
        sprintf(error_message, "Illegal Move: Black can't move White's piece");
        return true;
    } else if (!isupper(current_piece) && turn == WHITE_MOVE) {
        sprintf(error_message, "Illegal Move: White can't move Black's piece");
        return true;
    } else {
        return false;
    }
}

int is_legal_move(char *board, char cur_col, char cur_row, char new_col, char new_row, int turn) {
    if (is_square_off_board(cur_col, cur_row, new_col, new_row)) {
        return false;
    }
    if (is_current_square_empty(board, cur_col, cur_row)) {
        return false;
    }
    if (is_current_square_opponent_piece(board, cur_col, cur_row, turn)) {
        return false;
    }
    if (is_same_color(board, cur_col, cur_row, new_col, new_row)) {
        sprintf(error_message, "Illegal Move: destination contains your piece");
        return false;
    }
    switch(get_type_at_position(board, cur_col, cur_row)) {
        case TYPE_ROOK:
            return rook_move(board, cur_col, cur_row, new_col, new_row, turn);
            break;
        case TYPE_BISHOP:
            return bishop_move(board, cur_col, cur_row, new_col, new_row);
            break;
        case TYPE_QUEEN:
            return queen_move(board, cur_col, cur_row, new_col, new_row);
            break;
        case TYPE_KING:
            return king_move(board, cur_col, cur_row, new_col, new_row, turn);
            break;
        case TYPE_PAWN:
            return pawn_move(board, cur_col, cur_row, new_col, new_row, turn);
            break;
        case TYPE_KNIGHT:
            return knight_move(board, cur_col, cur_row, new_col, new_row);
            break;
        default:
            sprintf(error_message, "something went wrong\n");
            return false;
    }
}

int move_piece(char *board, char cur_col, char cur_row, char new_col, char new_row, int turn) {
    int legal = is_legal_move(board, cur_col, cur_row, new_col, new_row, turn);
    if (!legal) {
        return false;
    }
    set_piece_at_position(board, new_col, new_row, get_piece_at_position(board, cur_col, cur_row));
    set_piece_at_position(board, cur_col, cur_row, 0);
    en_passant_time = (en_passant_time > 0) ? en_passant_time - 1 : 0;
    return legal;
}
