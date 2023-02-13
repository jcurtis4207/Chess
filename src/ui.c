#include "ui.h"

/*
king    queen   bishop  knight  rook    pawn
 \+/     \w/
 ) (     ) (     (o)     ("\     [w]      _
 | |     | |     ) (     ) '     | |     ( )
/___\   /___\   /___\   /___\   /___\   /___\
*/

char king[] = " \\+/ \n ) ( \n | | \n/___\\";
char queen[] = " \\w/ \n ) ( \n | | \n/___\\";
char bishop[] = "     \n (o) \n ) ( \n/___\\";
char knight[] = "     \n (\"\\ \n ) ' \n/___\\";
char rook[] = "     \n [w] \n | | \n/___\\";
char pawn[] = "     \n  _  \n ( ) \n/___\\";

int square[BOX_SIZE];
WINDOW *ui;

int init_ui() {
    ui = initscr();
    if (ui == NULL) {
        fprintf(stderr, "UI failed to initialize\n");
        return false;
    }

    start_color();
    if (!has_colors() || !can_change_color()) {
        fprintf(stderr, "Terminal doesn't support colors\n");
        return false;
    }

    // disable cursor
    curs_set(0);

    // create colors
    init_pair(BLACK_PIECE, COLOR_RED, COLOR_BLACK);
    init_pair(WHITE_PIECE, COLOR_CYAN, COLOR_BLACK);
    init_pair(NORMAL, COLOR_WHITE, COLOR_BLACK);
    init_pair(DARK, COLOR_BLUE, COLOR_BLACK);
    wbkgd(ui, COLOR_PAIR(NORMAL));

    // create `box` of curses characters
    int tmp_square[BOX_SIZE] = {ACS_ULCORNER, ACS_HLINE, ACS_HLINE, ACS_HLINE, ACS_HLINE, ACS_HLINE, ACS_HLINE, ACS_HLINE, ACS_URCORNER, '\n',
                ACS_VLINE, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ACS_VLINE, '\n',
                ACS_VLINE, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ACS_VLINE, '\n',
                ACS_VLINE, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ACS_VLINE, '\n',
                ACS_VLINE, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ACS_VLINE, '\n',
                ACS_LLCORNER, ACS_HLINE, ACS_HLINE, ACS_HLINE, ACS_HLINE, ACS_HLINE, ACS_HLINE, ACS_HLINE, ACS_LRCORNER
    };
    memcpy(square, tmp_square, BOX_SIZE*sizeof(int));
    return true;
}

int destroy_ui() {
    delwin(ui);
    endwin();
    refresh();
    return 0;
}

char* get_piece_array(char piece) {
    switch(get_type_from_char(piece)) {
        case TYPE_BISHOP:
            return bishop;
        case TYPE_KING:
            return king;
        case TYPE_KNIGHT:
            return knight;
        case TYPE_PAWN:
            return pawn;
        case TYPE_QUEEN:
            return queen;
        case TYPE_ROOK:
            return rook;
        default:
            return NULL;
    }
}

void draw_piece(int x, int y, char piece, int color) {
    attrset(COLOR_PAIR(color));
    int x_start = x * SQUARE_WIDTH + 2;
    int ypos = y * SQUARE_HEIGHT + 1;
    int xpos = x_start;

    char* ascii_piece = get_piece_array(piece);

    for (int i = 0; i < PIECE_SIZE; i++) {
        if (ascii_piece[i] == '\0') {
            break;
        } else if (ascii_piece[i] == '\n') {
            ypos++;
            xpos = x_start;
        }
        else {
            mvaddch(ypos, X_OFFSET + xpos++, ascii_piece[i]);
        }
    }
}

void draw_all_pieces(char *board) {
    for (int row = 0; row < NUM_ROWS; row++) {
        for (int col = 0; col < NUM_ROWS; col++) {
            char piece = board[row * NUM_ROWS + col];
            if (piece == 0) {
                continue;
            } else if (isupper(piece)) {
                draw_piece(col, 7 - row, piece, WHITE_PIECE);
            } else {
                draw_piece(col, 7 - row, piece, BLACK_PIECE);
            }
        }
    }
}

void draw_square(int x, int y, int count) {
    int xpos = x;
    if (((count / NUM_ROWS % 2 == 0) && count % 2 == 0) ||
        ((count / NUM_ROWS % 2 == 1) && count % 2 == 1)) {
            attrset(COLOR_PAIR(NORMAL));
    } else {
        attrset(COLOR_PAIR(DARK));
    }
    for (int i = 0; i < BOX_SIZE; i++) {
        if (square[i] == '\n') {
            y++;
            xpos = x;
        } else {
            mvaddch(y, X_OFFSET + xpos++, square[i]);
        }
    }
}

void draw_all_squares() {
    int count = 0;
    for (int row = 0; row < NUM_ROWS; row++) {
        for (int col = 0; col < NUM_ROWS; col++) {
            draw_square(col * SQUARE_WIDTH, row * SQUARE_HEIGHT, count++);
        }
    }
}

void draw_labels() {
    attrset(COLOR_PAIR(NORMAL));
    char label = '8';
    for (int row = 0; row < NUM_ROWS; row++) {
        mvaddch(row * SQUARE_HEIGHT + 2, 2, label--);
    }
    label = 'A';
    for (int col = 0; col < NUM_ROWS; col++) {
        mvaddch(SQUARE_HEIGHT * 8, col * SQUARE_WIDTH + X_OFFSET + 4, label++);
    }
}

void draw_board(char *board) {
    erase();
    draw_all_squares();
    draw_all_pieces(board);
    draw_labels();
    refresh();
}

void draw_prompt(int turn) {
    attrset(COLOR_PAIR(NORMAL));
    mvprintw(51, 32, "Next Move (%s): ", (turn == WHITE_MOVE) ? "White" : "Black");
    refresh();
}

void read_input(char *cur_col, char *cur_row, char *new_col, char *new_row) {
    *cur_col = getchar();
    addch(*cur_col);
    refresh();
    *cur_row = getchar();
    addch(*cur_row);
    refresh();
    // separator
    getchar();
    addstr("->");
    refresh();
    *new_col = getchar();
    addch(*new_col);
    refresh();
    *new_row = getchar();
    addch(*new_row);
    refresh();
    getchar(); // enter
}

void print_error(char *error) {
    mvaddstr(53, 25, error);
    refresh();
    sleep(2);
}

void get_pawn_promotion_input(char *new_piece, int turn) {
    mvaddstr(51, 20, "Enter a piece for pawn promotion [q, r, b, n]: ");
    refresh();

    char input;
    int run = true;
    while (run) {
        input = getchar();
        switch(tolower(input)) {
            case 'q':
            case 'r':
            case 'b':
            case 'n':
                run = false;
                break;
            default:
                continue;
        }
    }

    if (turn == WHITE_MOVE) {
        *new_piece = toupper(input);
    } else {
        *new_piece = tolower(input);
    }
}
