#include <stdio.h>
#include "acutest.h"

#include "../src/board.h"
#include "../src/moves.h"

char starting_board[8][8] = {
    {'R','N','B','Q','K','B','N','R'},
    {'P','P','P','P','P','P','P','P'},
    { 0,  0,  0,  0,  0,  0,  0,  0 },
    { 0,  0,  0,  0,  0,  0,  0,  0 },
    { 0,  0,  0,  0,  0,  0,  0,  0 },
    { 0,  0,  0,  0,  0,  0,  0,  0 },
    {'p','p','p','p','p','p','p','p'},
    {'r','n','b','q','k','b','n','r'}
};

void test_piece_position() {
    char *board = &starting_board[0][0];

    struct TestVector {
        char* position;
        char expected;
    } vectors[] = {
        {"a1", 'R'},
        {"b1", 'N'},
        {"h8", 'r'},
        {"h7", 'p'},
        {"e4", 0}
    };

    for (int i = 0; i < sizeof(vectors) / sizeof(vectors[0]); i++) {
        struct TestVector* vec = &vectors[i];
        TEST_CASE(vec->position);
        TEST_CHECK(get_piece_at_position(board, vec->position[0], vec->position[1]) == vec->expected);
    }

    set_piece_at_position(board, 'e', '4', 'e');
    TEST_CHECK(get_piece_at_position(board, 'e', '4') == 'e');
    set_piece_at_position(board, 'e', '4', 0);
    TEST_CHECK(get_piece_at_position(board, 'e', '4') == 0);
}

void test_type_position() {
    char *board = &starting_board[0][0];

    struct TestVector {
        char* position;
        int expected;
        char* description;
    } vectors[] = {
        {"a1", TYPE_ROOK,   "white rook"},
        {"b1", TYPE_KNIGHT, "white knight"},
        {"c1", TYPE_BISHOP, "white bishop"},
        {"d1", TYPE_QUEEN,  "white queen"},
        {"e1", TYPE_KING,   "white king"},
        {"f2", TYPE_PAWN,   "white pawn"},
        {"h8", TYPE_ROOK,   "black rook"},
        {"g8", TYPE_KNIGHT, "black knight"},
        {"f8", TYPE_BISHOP, "black bishop"},
        {"e8", TYPE_KING,   "black queen"},
        {"d8", TYPE_QUEEN,  "black king"},
        {"c7", TYPE_PAWN,   "black pawn"}
    };

    for (int i = 0; i < sizeof(vectors) / sizeof(vectors[0]); i++) {
        struct TestVector* vec = &vectors[i];
        TEST_CASE(vec->description);
        TEST_CHECK(get_type_at_position(board, vec->position[0], vec->position[1]) == vec->expected);
    }
}

void test_illegal_positions() {
    char *board = &starting_board[0][0];

    struct TestVector {
        char* current;
        char* new;
        char* description;
    } vectors[] = {
        {"A1", "a1", "cur_col > max"},
        {"a1", "i1", "new_col > max"},
        {"a9", "a1", "cur_row > max"},
        {"a1", "a9", "new_row > max"},
        {"a0", "a1", "cur_row < min"},
        {"a1", "a0", "new_row < min"},
        {"91", "a1", "cur_col < min"},
        {"a1", "91", "new_col < min"}
    };

    for (int i = 0; i < sizeof(vectors) / sizeof(vectors[0]); i++) {
        struct TestVector* vec = &vectors[i];
        TEST_CASE(vec->description);
        TEST_CHECK(is_legal_move(board, vec->current[0], vec->current[1], vec->new[0], vec->new[1], WHITE_MOVE) == false);
    }
}

void test_move_empty() {
    char *board = &starting_board[0][0];

    struct TestVector {
        char* current;
        char* new;
    } vectors[] = {
        {"c1", "d1"},
        {"c1", "c1"},
        {"c1", "c5"}
    };

    for (int i = 0; i < sizeof(vectors) / sizeof(vectors[0]); i++) {
        struct TestVector* vec = &vectors[i];
        TEST_CASE(vec->new);
        TEST_CHECK(is_legal_move(board, vec->current[0], vec->current[1], vec->new[0], vec->new[1], WHITE_MOVE) == false);
    }
}

void test_move_opponent() {
    char *board = &starting_board[0][0];

    struct TestVector {
        char* current;
        char* new;
        int turn;
        char* description;
    } vectors[] = {
        {"h7", "h6", WHITE_MOVE, "white move black pawn"},
        {"g8", "f6", WHITE_MOVE, "white move black knight"},
        {"a2", "a3", BLACK_MOVE, "black move white pawn"},
        {"b1", "a3", BLACK_MOVE, "black move white knight"}
    };

    for (int i = 0; i < sizeof(vectors) / sizeof(vectors[0]); i++) {
        struct TestVector* vec = &vectors[i];
        TEST_CASE(vec->description);
        TEST_CHECK(is_legal_move(board, vec->current[0], vec->current[1], vec->new[0], vec->new[1], vec->turn) == false);
    }
}

void test_take_own_piece() {
    char *board = &starting_board[0][0];

    struct TestVector {
        char* current;
        char* new;
        int turn;
        char* description;
    } vectors[] = {
        {"a1", "b1", WHITE_MOVE, "white rook take knight"},
        {"b1", "b1", WHITE_MOVE, "white pawn take itself"},
        {"e8", "d8", BLACK_MOVE, "black king take queen"},
        {"h8", "h7", BLACK_MOVE, "black rook take pawn"}
    };

    for (int i = 0; i < sizeof(vectors) / sizeof(vectors[0]); i++) {
        struct TestVector* vec = &vectors[i];
        TEST_CASE(vec->description);
        TEST_CHECK(is_legal_move(board, vec->current[0], vec->current[1], vec->new[0], vec->new[1], vec->turn) == false);
    }
}

void test_rook_moves() {
    char rook_board[8][8] = {
        {'R', 0,  0,  0,  0,  0,  0,  0 },
        { 0,  0,  0,  0,  0,  0,  0,  0 },
        { 0,  0,  0, 'p', 0,  0,  0,  0 },
        { 0,  0, 'p','R','p', 0,  0,  0 },
        { 0,  0,  0, 'p', 0,  0,  0,  0 },
        { 0,  0,  0,  0,  0,  0,  0,  0 },
        { 0,  0,  0,  0,  0,  0,  0,  0 },
        { 0,  0,  0,  0,  0,  0,  0, 'r'}
    };
    char *board = &rook_board[0][0];

    struct TestVector {
        char* current;
        char* new;
        int turn;
        int pass;
        char* description;
    } vectors[] = {
        {"a1", "a4", WHITE_MOVE, true,  "up->yes"},
        {"a1", "f1", WHITE_MOVE, true,  "right->yes"},
        {"h8", "h6", BLACK_MOVE, true,  "down->yes"},
        {"h8", "a8", BLACK_MOVE, true,  "left->yes"},
        {"a1", "b2", WHITE_MOVE, false, "white diagonal->no"},
        {"h8", "g7", BLACK_MOVE, false, "black diagonal->no"},
        {"d4", "d2", WHITE_MOVE, false, "down blocked->no"},
        {"d4", "d6", WHITE_MOVE, false, "up blocked->no"},
        {"d4", "b4", WHITE_MOVE, false, "left blocked->no"},
        {"d4", "g4", WHITE_MOVE, false, "right blocked->no"},
        {"a1", "c2", WHITE_MOVE, false, "knight move->no"}
    };

    for (int i = 0; i < sizeof(vectors) / sizeof(vectors[0]); i++) {
        struct TestVector* vec = &vectors[i];
        TEST_CASE(vec->description);
        TEST_CHECK(is_legal_move(board, vec->current[0], vec->current[1], vec->new[0], vec->new[1], vec->turn) == vec->pass);
    }
}

void test_bishop_moves() {
    char bishop_board[8][8] = {
        { 0,  0,  0,  0,  0,  0,  0,  0 },
        { 0,  0,  0,  0,  0,  0,  0,  0 },
        { 0,  0, 'p', 0, 'p', 0,  0,  0 },
        { 0,  0,  0, 'B', 0,  0,  0,  0 },
        { 0, 'B','p', 0, 'p', 0,  0,  0 },
        { 0,  0,  0,  0,  0,  0,  0,  0 },
        { 0,  0,  0,  0,  0,  0,  0,  0 },
        { 0,  0,  0,  0,  0,  0,  0, 'b'}
    };
    char *board = &bishop_board[0][0];

    struct TestVector {
        char* current;
        char* new;
        int turn;
        int pass;
        char* description;
    } vectors[] = {
        {"b5", "a6", WHITE_MOVE, true,  "up/left->yes"},
        {"b5", "a4", WHITE_MOVE, true,  "down/left->yes"},
        {"b5", "e8", WHITE_MOVE, true,  "up/right->yes"},
        {"b5", "f1", WHITE_MOVE, true,  "down/right->yes"},
        {"h8", "g7", BLACK_MOVE, true,  "black unblocked->yes"},
        {"h8", "a1", BLACK_MOVE, false, "black blocked->no"},
        {"d4", "a1", WHITE_MOVE, false, "down/left blocked->no"},
        {"d4", "a7", WHITE_MOVE, false, "up/left blocked->no"},
        {"d4", "g1", WHITE_MOVE, false, "down/right blocked->no"},
        {"d4", "g8", WHITE_MOVE, false, "up/right blocked->no"},
        {"d4", "d5", WHITE_MOVE, false, "up->no"},
        {"d4", "d3", WHITE_MOVE, false, "down->no"},
        {"d4", "e4", WHITE_MOVE, false, "right->no"},
        {"d4", "c4", WHITE_MOVE, false, "left->no"},
        {"b5", "a7", WHITE_MOVE, false, "knight move->no"}
    };

    for (int i = 0; i < sizeof(vectors) / sizeof(vectors[0]); i++) {
        struct TestVector* vec = &vectors[i];
        TEST_CASE(vec->description);
        TEST_CHECK(is_legal_move(board, vec->current[0], vec->current[1], vec->new[0], vec->new[1], vec->turn) == vec->pass);
    }
}

void test_queen_moves() {
    char queen_board[8][8] = {
        { 0,  0,  0,  0,  0,  0,  0,  0 },
        { 0,  0,  0, 'p', 0,  0,  0,  0 },
        { 0,  0, 'p', 0,  0,  0,  0,  0 },
        { 0,  0,  0, 'Q', 0,  0,  0,  0 },
        { 0, 'Q','p', 0,  0,  0,  0,  0 },
        { 0,  0,  0,  0,  0,  0,  0,  0 },
        { 0,  0,  0, 'p', 0,  0,  0,  0 },
        { 0,  0,  0,  0,  0,  0,  0, 'q'}
    };
    char *board = &queen_board[0][0];

    struct TestVector {
        char* current;
        char* new;
        int turn;
        int pass;
        char* description;
    } vectors[] = {
        {"b5", "b1", WHITE_MOVE, true,  "down->yes"},
        {"b5", "b8", WHITE_MOVE, true,  "up->yes"},
        {"b5", "a5", WHITE_MOVE, true,  "left->yes"},
        {"b5", "a6", WHITE_MOVE, true,  "up/left->yes"},
        {"b5", "a4", WHITE_MOVE, true,  "down/left->yes"},
        {"b5", "c6", WHITE_MOVE, true,  "up/right->yes"},
        {"b5", "f1", WHITE_MOVE, true,  "down/right->yes"},
        {"h8", "h1", BLACK_MOVE, true,  "black down->yes"},
        {"h8", "a8", BLACK_MOVE, true,  "black right->yes"},
        {"h8", "e5", BLACK_MOVE, true,  "black diagonal->yes"},
        {"h8", "b2", BLACK_MOVE, false, "black blocked->no"},
        {"b5", "d5", WHITE_MOVE, false, "right blocked->no"},
        {"d4", "a1", WHITE_MOVE, false, "down/left blocked->no"},
        {"d4", "d1", WHITE_MOVE, false, "down blocked->no"},
        {"d4", "d8", WHITE_MOVE, false, "up blocked->no"},
        {"d4", "a7", WHITE_MOVE, false, "up/left blocked->no"},
        {"d4", "f3", WHITE_MOVE, false, "knight down->no"},
        {"d4", "f5", WHITE_MOVE, false, "knight up->no"}
    };

    for (int i = 0; i < sizeof(vectors) / sizeof(vectors[0]); i++) {
        struct TestVector* vec = &vectors[i];
        TEST_CASE(vec->description);
        TEST_CHECK(is_legal_move(board, vec->current[0], vec->current[1], vec->new[0], vec->new[1], vec->turn) == vec->pass);
    }
}

void test_king_moves() {
    char king_board[8][8] = {
        { 0,  0,  0,  0,  0,  0,  0,  0 },
        { 0,  0,  0, 'p', 0,  0,  0,  0 },
        { 0,  0, 'p', 0,  0,  0,  0,  0 },
        { 0,  0,  0,  0,  0,  0,  0,  0 },
        { 0, 'K','p', 0,  0,  0,  0,  0 },
        { 0,  0,  0,  0,  0,  0,  0,  0 },
        { 0,  0,  0, 'p', 0,  0,  0,  0 },
        { 0,  0,  0,  0,  0,  0,  0, 'k'}
    };
    char *board = &king_board[0][0];

    struct TestVector {
        char* current;
        char* new;
        int turn;
        int pass;
        char* description;
    } vectors[] = {
        {"b5", "a4", WHITE_MOVE, true,  "down/left->yes"},
        {"b5", "b4", WHITE_MOVE, true,  "down->yes"},
        {"b5", "c4", WHITE_MOVE, true,  "down/right->yes"},
        {"b5", "c5", WHITE_MOVE, true,  "right->yes"},
        {"b5", "c6", WHITE_MOVE, true,  "up/right->yes"},
        {"b5", "b6", WHITE_MOVE, true,  "up->yes"},
        {"b5", "a6", WHITE_MOVE, true,  "up/left->yes"},
        {"b5", "a5", WHITE_MOVE, true,  "left->yes"},
        {"h8", "h7", BLACK_MOVE, true,  "black down->yes"},
        {"h8", "g8", BLACK_MOVE, true,  "black left->yes"},
        {"h8", "g7", BLACK_MOVE, true,  "black diagonal->yes"},
        {"h8", "h6", BLACK_MOVE, false, "black down 2->no"},
        {"h8", "f8", BLACK_MOVE, false, "black left 2->no"},
        {"b5", "b1", WHITE_MOVE, false, "down 4->no"},
        {"b5", "d5", WHITE_MOVE, false, "right 2->no"},
        {"d4", "a7", WHITE_MOVE, false, "diagonal 3->no"}
    };

    for (int i = 0; i < sizeof(vectors) / sizeof(vectors[0]); i++) {
        struct TestVector* vec = &vectors[i];
        TEST_CASE(vec->description);
        TEST_CHECK(is_legal_move(board, vec->current[0], vec->current[1], vec->new[0], vec->new[1], vec->turn) == vec->pass);
    }
}

void test_pawn_moves() {
    char pawn_board[8][8] = {
        { 0,  0,  0,  0, 'b', 0,  0,  0 },
        { 0, 'P', 0, 'P', 0,  0,  0,  0 },
        {'P', 0,  0, 'N','b','b', 0,  0 },
        { 0,  0,  0,  0,  0,  0,  0,  0 },
        { 0,  0,  0,  0,  0,  0,  0,  0 },
        { 0,  0,  0, 'B','n', 0,  0, 'p'},
        { 0,  0,  0,  0, 'p', 0, 'p', 0 },
        { 0,  0,  0, 'B', 0,  0,  0,  0 }
    };
    char *board = &pawn_board[0][0];

    struct TestVector {
        char* current;
        char* new;
        int turn;
        int pass;
        char* description;
    } vectors[] = {
        {"b2", "b3", WHITE_MOVE, true,  "white forward 1->yes"},
        {"b2", "b1", WHITE_MOVE, false, "white backward 1->no"},
        {"b2", "c2", WHITE_MOVE, false, "white right 1->no"},
        {"b2", "a2", WHITE_MOVE, false, "white left 1->no"},
        {"d2", "d3", WHITE_MOVE, false, "white blocked->no"},
        {"g7", "g6", BLACK_MOVE, true,  "black forward 1->yes"},
        {"g7", "g8", BLACK_MOVE, false, "black backward 1->no"},
        {"g7", "h7", BLACK_MOVE, false, "black right 1->no"},
        {"g7", "f7", BLACK_MOVE, false, "black left 1->no"},
        {"e7", "e6", BLACK_MOVE, false, "black blocked->no"},
        {"a3", "a5", WHITE_MOVE, false, "white forward 2->no"},
        {"h6", "h4", BLACK_MOVE, false, "black forward 2->no"},
        {"b2", "b4", WHITE_MOVE, true,  "white init 2->yes"},
        {"g7", "g5", BLACK_MOVE, true,  "black init 2->yes"},
        {"d2", "e3", WHITE_MOVE, true,  "white diagonal take->yes"},
        {"d2", "f3", WHITE_MOVE, false, "white L take->no"},
        {"d2", "c3", WHITE_MOVE, false, "white diagonal no take->no"},
        {"d2", "e1", WHITE_MOVE, false, "white diagonal back take->no"},
        {"e7", "d6", BLACK_MOVE, true,  "black diagonal take->yes"},
        {"e7", "f7", BLACK_MOVE, false, "black diagonal no take->no"},
        {"e7", "d8", BLACK_MOVE, false, "black diagonal back take->no"}
    };

    for (int i = 0; i < sizeof(vectors) / sizeof(vectors[0]); i++) {
        struct TestVector* vec = &vectors[i];
        TEST_CASE(vec->description);
        TEST_CHECK(is_legal_move(board, vec->current[0], vec->current[1], vec->new[0], vec->new[1], vec->turn) == vec->pass);
    }
}

void test_knight_moves() {
    char knight_board[8][8] = {
        { 0,  0,  0,  0,  0,  0,  0,  0 },
        { 0,  0,  0,  0,  0,  0,  0,  0 },
        { 0,  0,  0,  0,  0,  0,  0,  0 },
        { 0,  0,  0, 'N', 0,  0,  0,  0 },
        { 0,  0,  0,  0, 'n', 0,  0,  0 },
        { 0,  0,  0,  0,  0,  0,  0,  0 },
        { 0,  0,  0,  0,  0,  0,  0,  0 },
        { 0,  0,  0,  0,  0,  0,  0,  0 }
    };
    char *board = &knight_board[0][0];

    struct TestVector {
        char* current;
        char* new;
        int turn;
        int pass;
        char* description;
    } vectors[] = {
        {"d4", "c2", WHITE_MOVE, true,  "white pos 1->yes"},
        {"d4", "b3", WHITE_MOVE, true,  "white pos 2->yes"},
        {"d4", "b5", WHITE_MOVE, true,  "white pos 3->yes"},
        {"d4", "c6", WHITE_MOVE, true,  "white pos 4->yes"},
        {"d4", "e6", WHITE_MOVE, true,  "white pos 5->yes"},
        {"d4", "f5", WHITE_MOVE, true,  "white pos 6->yes"},
        {"d4", "f3", WHITE_MOVE, true,  "white pos 7->yes"},
        {"d4", "e2", WHITE_MOVE, true,  "white pos 8->yes"},
        {"e5", "d3", BLACK_MOVE, true,  "black pos 1->yes"},
        {"e5", "c4", BLACK_MOVE, true,  "black pos 2->yes"},
        {"e5", "c6", BLACK_MOVE, true,  "black pos 3->yes"},
        {"e5", "d7", BLACK_MOVE, true,  "black pos 4->yes"},
        {"e5", "f7", BLACK_MOVE, true,  "black pos 5->yes"},
        {"e5", "g6", BLACK_MOVE, true,  "black pos 6->yes"},
        {"e5", "g4", BLACK_MOVE, true,  "black pos 7->yes"},
        {"e5", "f3", BLACK_MOVE, true,  "black pos 8->yes"},
        {"d4", "a4", WHITE_MOVE, false, "left->no"},
        {"d4", "h4", WHITE_MOVE, false, "right->no"},
        {"d4", "h8", WHITE_MOVE, false, "up->no"},
        {"d4", "d1", WHITE_MOVE, false, "down->no"},
        {"d4", "b2", WHITE_MOVE, false, "down/left 2->no"},
        {"d4", "a1", WHITE_MOVE, false, "down/left 3->no"},
        {"d4", "c1", WHITE_MOVE, false, "pos 1 over->no"},
        {"d4", "a3", WHITE_MOVE, false, "pos 2 over->no"}
    };

    for (int i = 0; i < sizeof(vectors) / sizeof(vectors[0]); i++) {
        struct TestVector* vec = &vectors[i];
        TEST_CASE(vec->description);
        TEST_CHECK(is_legal_move(board, vec->current[0], vec->current[1], vec->new[0], vec->new[1], vec->turn) == vec->pass);
    }
}

void test_move_function() {
    char move_board[8][8] = {
        {'R','N','B','Q','K','B','N','R'},
        {'P','P','P','P','P','P','P','P'},
        { 0,  0,  0,  0,  0,  0,  0,  0 },
        { 0,  0,  0,  0,  0,  0,  0,  0 },
        { 0,  0,  0,  0,  0,  0,  0,  0 },
        { 0,  0,  0,  0,  0,  0,  0,  0 },
        {'p','p','p','p','p','p','p','p'},
        {'r','n','b','q','k','b','n','r'}
    };
    char *board = &move_board[0][0];

    // bKnight to C3
    TEST_ASSERT(move(board, 'b', '1', 'c', '3', WHITE_MOVE));
        TEST_ASSERT(get_piece_at_position(board, 'b', '1') == 0);
        TEST_ASSERT(get_piece_at_position(board, 'c', '3') == 'N');
    // dPawn to D5
    TEST_ASSERT(move(board, 'd', '7', 'd', '5', BLACK_MOVE));
        TEST_ASSERT(get_piece_at_position(board, 'd', '7') == 0);
        TEST_ASSERT(get_piece_at_position(board, 'd', '5') == 'p');
    // bKnight takes D5
    TEST_ASSERT(move(board, 'c', '3', 'd', '5', WHITE_MOVE));
        TEST_ASSERT(get_piece_at_position(board, 'c', '3') == 0);
        TEST_ASSERT(get_piece_at_position(board, 'd', '5') == 'N');
    // cBishop to E6
    TEST_ASSERT(move(board, 'c', '8', 'e', '6', BLACK_MOVE));
        TEST_ASSERT(get_piece_at_position(board, 'c', '8') == 0);
        TEST_ASSERT(get_piece_at_position(board, 'e', '6') == 'b');
    // cBishop takes D5
    TEST_ASSERT(move(board, 'e', '6', 'd', '5', BLACK_MOVE));
        TEST_ASSERT(get_piece_at_position(board, 'e', '6') == 0);
        TEST_ASSERT(get_piece_at_position(board, 'd', '5') == 'b');
}

void test_en_passant_yes() {
    char ep_board[8][8] = {
        {'R','N','B','Q','K','B','N','R'},
        {'P', 0, 'P','P','P','P','P','P'},
        { 0,  0,  0,  0,  0,  0,  0,  0 },
        { 0,  0,  0,  0,  0, 'p', 0,  0 },
        { 0, 'P', 0,  0,  0,  0,  0,  0 },
        { 0,  0,  0,  0,  0,  0,  0,  0 },
        {'p','p','p','p','p', 0, 'p','p'},
        {'r','n','b','q','k','b','n','r'}
    };
    char *board = &ep_board[0][0];
    // white
    move(board, 'c', '7', 'c', '5', BLACK_MOVE);
    TEST_ASSERT(en_passant_target_square[0] == 'c' && en_passant_target_square[1] == '6');
    TEST_ASSERT(en_passant_victim_square[0] == 'c' && en_passant_victim_square[1] == '5');
    TEST_ASSERT(en_passant_time == 1);
    TEST_ASSERT(move(board, 'b', '5', 'c', '6', WHITE_MOVE) == 1);
    TEST_ASSERT(get_piece_at_position(board, 'c', '5') == 0);
    TEST_ASSERT(en_passant_time == 0);
    // black
    move(board, 'e', '2', 'e', '4', WHITE_MOVE);
    TEST_ASSERT(en_passant_target_square[0] == 'e' && en_passant_target_square[1] == '3');
    TEST_ASSERT(en_passant_victim_square[0] == 'e' && en_passant_victim_square[1] == '4');
    TEST_ASSERT(en_passant_time == 1);
    TEST_ASSERT(move(board, 'f', '4', 'e', '3', BLACK_MOVE) == 1);
    TEST_ASSERT(get_piece_at_position(board, 'e', '4') == 0);
    TEST_ASSERT(en_passant_time == 0);
}

void test_en_passant_no() {
    char ep_board[8][8] = {
        {'R','N','B','Q','K','B','N','R'},
        {'P', 0, 'P','P','P','P','P','P'},
        { 0,  0,  0,  0,  0,  0,  0,  0 },
        { 0,  0,  0,  0,  0,  0,  0,  0 },
        { 0, 'P','P', 0,  0, 'p', 0,  0 },
        { 0,  0,  0,  0,  0,  0,  0,  0 },
        {'p','p','p','p','p', 0, 'p','p'},
        {'r','n','b','q','k','b','n','r'}
    };
    char *board = &ep_board[0][0];
    // only perform on next move
    move(board, 'c', '7', 'c', '5', BLACK_MOVE);
    TEST_ASSERT(en_passant_target_square[0] == 'c' && en_passant_target_square[1] == '6');
    TEST_ASSERT(en_passant_victim_square[0] == 'c' && en_passant_victim_square[1] == '5');
    move(board, 'a', '2', 'a', '3', WHITE_MOVE); // extra move
    move(board, 'a', '7', 'a', '6', BLACK_MOVE); // extra move
    TEST_ASSERT(en_passant_time == 0);
    TEST_ASSERT(move(board, 'b', '5', 'c', '6', WHITE_MOVE) == 0);
    // must be in same row
    move(board, 'e', '2', 'e', '4', WHITE_MOVE);
    TEST_ASSERT(en_passant_target_square[0] == 'e' && en_passant_target_square[1] == '3');
    TEST_ASSERT(en_passant_victim_square[0] == 'e' && en_passant_victim_square[1] == '4');
    TEST_ASSERT(move(board, 'f', '5', 'e', '3', BLACK_MOVE) == 0);
    // must be in adjacent column
    move(board, 'e', '7', 'e', '5', BLACK_MOVE);
    TEST_ASSERT(en_passant_target_square[0] == 'e' && en_passant_target_square[1] == '6');
    TEST_ASSERT(en_passant_victim_square[0] == 'e' && en_passant_victim_square[1] == '5');
    TEST_ASSERT(move(board, 'c', '5', 'e', '6', WHITE_MOVE) == 0);
}

TEST_LIST = {
    { "Piece Position", test_piece_position },
    { "Type Position", test_type_position },
    { "Illegal Positions", test_illegal_positions },
    { "Move Empty Square", test_move_empty },
    { "Move Opponent's Piece", test_move_opponent },
    { "Take Own Piece", test_take_own_piece },
    { "Rook Moves", test_rook_moves },
    { "Bishop Moves", test_bishop_moves },
    { "Queen Moves", test_queen_moves },
    { "King Moves", test_king_moves },
    { "Pawn Moves", test_pawn_moves },
    { "Knight Moves", test_knight_moves },
    { "Move Function", test_move_function },
    { "En Passant Positive", test_en_passant_yes },
    { "En Passant Negative", test_en_passant_no },
    { NULL, NULL }
};
