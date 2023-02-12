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
    int vertical_offset = new_row - cur_row;
    int horizontal_offset = new_col - cur_col;
    if (vertical_offset != 0 && horizontal_offset != 0){
        fprintf(stderr, "Illegal Move: move is not linear\n");
        return false;
    }
    if (vertical_offset != 0) {
        return is_column_clear(board, cur_col, cur_row, new_col, new_row);
    } else {
        return is_row_clear(board, cur_col, cur_row, new_col, new_row);
    }
}

int diagonal_move(char *board, char cur_col, char cur_row, char new_col, char new_row) {
    int vertical_offset = new_row - cur_row;
    int horizontal_offset = new_col - cur_col;
    if (abs(vertical_offset) != abs(horizontal_offset)){
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
    int vertical_offset = new_row - cur_row;
    int horizontal_offset = new_col - cur_col;
    if (abs(horizontal_offset) > 1 || abs(vertical_offset) > 1) {
        fprintf(stderr, "Illegal Move: king cannot more than 1 space\n");
        return false;
    }
    if (diagonal_move(board, cur_col, cur_row, new_col, new_row)) {
        return true;
    } else {
        return linear_move(board, cur_col, cur_row, new_col, new_row);
    }
}

int pawn_move(char *board, char cur_col, char cur_row, char new_col, char new_row, int turn) {
    int vertical_offset = new_row - cur_row;
    int horizontal_offset = new_col - cur_col;
    if (abs(vertical_offset) > 1) {
        // can only move 2 spaces on first move
        if ((turn == WHITE_MOVE && cur_row == '2' && vertical_offset == 2) ||
            (turn == BLACK_MOVE && cur_row == '7' && vertical_offset == -2)) {
                return linear_move(board, cur_col, cur_row, new_col, new_row);
        } else {
            fprintf(stderr, "Illegal Move: pawn cannot move more than 1 space after its first move\n");
            return false;
        }
    } else if (vertical_offset > 0 && turn == BLACK_MOVE) {
        fprintf(stderr, "Illegal Move: pawn cannot move backwards\n");
        return false;
    } else if (vertical_offset < 0 && turn == WHITE_MOVE) {
        fprintf(stderr, "Illegal Move: pawn cannot move backwards\n");
        return false;
    // can only move horizontally when taking
    } else if (abs(horizontal_offset) > 0) {
        if (abs(horizontal_offset) == 1 &&
            get_type_at_position(board, new_col, new_row) != TYPE_EMPTY &&
            !is_same_color(board, cur_col, cur_row, new_col, new_row)) {
            return true;
        } else {
            fprintf(stderr, "Illegal Move: pawn cannot move horiztonally\n");
            return false;
        }
    } else {
        return linear_move(board, cur_col, cur_row, new_col, new_row);
    }
}

int knight_move(char *board, char cur_col, char cur_row, char new_col, char new_row) {
    int vertical_offset = abs(new_row - cur_row);
    int horizontal_offset = abs(new_col - cur_col);
    if (vertical_offset == horizontal_offset) {
        fprintf(stderr, "Illegal Move: knight cannot move diagonally\n");
        return false;
    } else if (vertical_offset == 0 || horizontal_offset == 0) {
        fprintf(stderr, "Illegal Move: knight cannot move linearly\n");
        return false;
    } else if (vertical_offset + horizontal_offset != 3) {
        fprintf(stderr, "Illegal Move: knight must move 3 spaces\n");
        return false;
    }
    return true;
}

int is_legal_move(char *board, char cur_col, char cur_row, char new_col, char new_row, int turn) {
    // can't move off of board
    if (cur_col < 'a' || cur_col > 'h' || cur_row < '1' || cur_row > '8' ||
        new_col < 'a' || new_col > 'h' || new_row < '1' || new_row > '8') {
        fprintf(stderr, "Illegal Move: position outside of range\n");
        return false;
    }
    char current_piece = get_piece_at_position(board, cur_col, cur_row);
    // can't move from an empty space
    if (current_piece == TYPE_EMPTY) {
        fprintf(stderr, "Illegal Move: no piece at current position\n");
        return false;
    }
    // can't move your opponent's piece
    if (isupper(current_piece) && turn == BLACK_MOVE) {
        fprintf(stderr, "Illegal Move: Black can't move White's piece\n");
        return false;
    }
    if (!isupper(current_piece) && turn == WHITE_MOVE) {
        fprintf(stderr, "Illegal Move: White can't move Black's piece\n");
        return false;
    }
    // can't move to a space occupied by your piece
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
