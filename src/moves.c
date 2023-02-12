#include "moves.h"

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
            fprintf(stderr, "Illegal Move: column is not clear\n");
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
            fprintf(stderr, "Illegal Move: row is not clear\n");
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
            fprintf(stderr, "Illegal Move: diagonal is not clear\n");
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
    if (vertical_distance != 0 && horizontal_distance != 0){
        fprintf(stderr, "Illegal Move: move is not linear\n");
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
    if (abs(vertical_distance) != abs(horizontal_distance)){
        fprintf(stderr, "Illegal Move: move is not diagonal\n");
        return false;
    }
    return is_diagonal_clear(board, cur_col, cur_row, new_col, new_row);
}

int rook_move(char *board, char cur_col, char cur_row, char new_col, char new_row) {
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

int king_move(char *board, char cur_col, char cur_row, char new_col, char new_row) {
    int vertical_distance = new_row - cur_row;
    int horizontal_distance = new_col - cur_col;
    if (abs(horizontal_distance) > 1 || abs(vertical_distance) > 1) {
        fprintf(stderr, "Illegal Move: king cannot more than 1 space\n");
        return false;
    }
    if (diagonal_move(board, cur_col, cur_row, new_col, new_row)) {
        return true;
    } else {
        return linear_move(board, cur_col, cur_row, new_col, new_row);
    }
}

int check_pawn_direction(int vertical_distance, int turn) {
    if (vertical_distance > 0 && turn == BLACK_MOVE) {
        fprintf(stderr, "Illegal Move: pawn cannot move backwards\n");
        return false;
    } else if (vertical_distance < 0 && turn == WHITE_MOVE) {
        fprintf(stderr, "Illegal Move: pawn cannot move backwards\n");
        return false;
    } else {
        return true;
    }
}

int check_pawn_distance(char cur_row, int vertical_distance, int turn) {
    if (abs(vertical_distance) > 1) {
        // can only move 2 spaces on first move
        if ((turn == WHITE_MOVE && cur_row == '2' && vertical_distance == 2) ||
            (turn == BLACK_MOVE && cur_row == '7' && vertical_distance == -2)) {
                return true;
        } else {
            fprintf(stderr, "Illegal Move: pawn cannot move more than 1 space after its first move\n");
            return false;
        }
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
    } else {
        fprintf(stderr, "Illegal Move: pawn cannot move horiztonally\n");
        return false;
    }
}

int pawn_move(char *board, char cur_col, char cur_row, char new_col, char new_row, int turn) {
    int vertical_distance = new_row - cur_row;
    int horizontal_distance = new_col - cur_col;
    if (!check_pawn_direction(vertical_distance, turn)) {
        return false;
    }
    if (!check_pawn_distance(cur_row, vertical_distance, turn)) {
        return false;
    }
    if (abs(horizontal_distance) > 0) {
        if (check_pawn_taking(board, cur_col, cur_row, new_col, new_row)) {
            return true;
        } else {
            return false;
        }
    }
    return linear_move(board, cur_col, cur_row, new_col, new_row);
}

int knight_move(char *board, char cur_col, char cur_row, char new_col, char new_row) {
    int vertical_distance = abs(new_row - cur_row);
    int horizontal_distance = abs(new_col - cur_col);
    if (vertical_distance == horizontal_distance) {
        fprintf(stderr, "Illegal Move: knight cannot move diagonally\n");
        return false;
    } else if (vertical_distance == 0 || horizontal_distance == 0) {
        fprintf(stderr, "Illegal Move: knight cannot move linearly\n");
        return false;
    } else if (vertical_distance + horizontal_distance != 3) {
        fprintf(stderr, "Illegal Move: knight must move 3 spaces\n");
        return false;
    }
    return true;
}

int is_square_off_board(char cur_col, char cur_row, char new_col, char new_row) {
    if (cur_col < 'a' || cur_col > 'h' || cur_row < '1' || cur_row > '8' ||
        new_col < 'a' || new_col > 'h' || new_row < '1' || new_row > '8') {
        fprintf(stderr, "Illegal Move: position outside of range\n");
        return true;
    } else {
        return false;
    }
}

int is_current_square_empty(char *board, char cur_col, char cur_row) {
    char current_square = get_piece_at_position(board, cur_col, cur_row);
    if (current_square == TYPE_EMPTY) {
        fprintf(stderr, "Illegal Move: no piece at current position\n");
        return true;
    } else {
        return false;
    }
}

int is_current_square_opponent_piece(char *board, char cur_col, char cur_row, int turn) {
    char current_piece = get_piece_at_position(board, cur_col, cur_row);
    if (isupper(current_piece) && turn == BLACK_MOVE) {
        fprintf(stderr, "Illegal Move: Black can't move White's piece\n");
        return true;
    } else if (!isupper(current_piece) && turn == WHITE_MOVE) {
        fprintf(stderr, "Illegal Move: White can't move Black's piece\n");
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
        fprintf(stderr, "Illegal Move: destination contains your piece\n");
        return false;
    }
    switch(get_type_at_position(board, cur_col, cur_row)) {
        case TYPE_ROOK:
            return rook_move(board, cur_col, cur_row, new_col, new_row);
            break;
        case TYPE_BISHOP:
            return bishop_move(board, cur_col, cur_row, new_col, new_row);
            break;
        case TYPE_QUEEN:
            return queen_move(board, cur_col, cur_row, new_col, new_row);
            break;
        case TYPE_KING:
            return king_move(board, cur_col, cur_row, new_col, new_row);
            break;
        case TYPE_PAWN:
            return pawn_move(board, cur_col, cur_row, new_col, new_row, turn);
            break;
        case TYPE_KNIGHT:
            return knight_move(board, cur_col, cur_row, new_col, new_row);
            break;
        default:
            fprintf(stderr, "something went wrong\n");
            return false;
    }
}

int move(char *board, char cur_col, char cur_row, char new_col, char new_row, int turn) {
    if (!is_legal_move(board, cur_col, cur_row, new_col, new_row, turn)) {
        return false;
    }
    set_piece_at_position(board, new_col, new_row, get_piece_at_position(board, cur_col, cur_row));
    set_piece_at_position(board, cur_col, cur_row, 0);
    return true;
}
