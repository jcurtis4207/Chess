CC=gcc
CFLAGS=-Wall
BIN=build/chess.bin
MAIN=src/main.c
SRC=src/moves.c src/board.c
OBJ=build/moves.o build/board.o
TESTS=tests/test.c
TESTBIN=tests/test.bin

all: $(BIN)

$(BIN): $(OBJ) $(MAIN)
	$(CC) $(CFLAGS) $(OBJ) $(MAIN) -o $@

build/%.o: src/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

run: $(BIN)
	$(BIN)

test: $(TESTBIN)
	tests/test.bin 2>/dev/null

$(TESTBIN): $(TESTS) $(OBJ)
	$(CC) $(CLFAGS) $< $(OBJ) -o $@

clean:
	$(RM) -r build
	$(RM) $(TESTBIN)
