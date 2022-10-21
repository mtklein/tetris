#include <stdint.h>
#include <stdio.h>

// 4-row Wordle boards are represented by 15 bits in a uint16_t,
//
//    0b abcde'fghij'klmno
//      14         5     0
//
// where abcde (bits 14-10) are the top row, left to right,
// fghij (bits 9-5) the second row and klmno (bits 4-0) the third row.
//
// A green box is represented by a 1 bit, black a 0 bit.

static bool is_green(uint16_t board, int i) { return board & (1<<i); }

// The bottom fourth row is assumed to be all green, 11111.
//
// So for example, 0b00000'00010'00110 represents this board:
//
//      ⬛⬛⬛⬛⬛
//      ⬛⬛⬛🟩⬛
//      ⬛⬛🟩🟩⬛
//      🟩🟩🟩🟩🟩

// In Wordle, if we win on row 4, we can't have won on earlier rows.
static bool no_all_green_rows(uint16_t board) {
    return (board & 0b00000'00000'11111) != 0b00000'00000'11111   // Not won on third row,
        && (board & 0b00000'11111'00000) != 0b00000'11111'00000   // not won on second row,
        && (board & 0b11111'00000'00000) != 0b11111'00000'00000;  // and not won on top row.
}

// To be tetrizable, the total number of green boxes must at least divide evenly by 4.
// There are always 5 green boxes on row 4, so the other rows need to total 3, 7, or 11.
// (15 could work here too but that'll be ruled out by no_all_green_rows().)
static bool congruent_to_3_mod_4(uint16_t board) {
    return __builtin_popcount(board) % 4 == 3;
}

// In strict Wordle, once a box is green, it stays green on subsequent rows.
static bool strict(uint16_t board) {
    // No black box on the second or third row underneath a green box on the row above it.
    for (int i = 0; i < 10; i++) {
        if (!is_green(board, i) && is_green(board, i+5)) { return false; }
    }
    return true;
}

static bool maybe_legal(uint16_t board) {
    return congruent_to_3_mod_4(board)  // 8255 possible boards with just this constraint
        && no_all_green_rows(board)     // 7505 with this constraint too
        && strict(board);               //  195 requiring strict boards
}

static void print_board(uint16_t board) {
    printf("%04x %s\n", board, maybe_legal(board) ? "ok" : "NO");
    for (int i = 15; i --> 0;) {
        printf("%s%s", (board & (1<<i)) ? "🟩" : "⬛"
                     , (i % 5 == 0) ? "\n" : "");
    }
    printf("🟩🟩🟩🟩🟩\n\n");
}

int main(void) {
    // Run some simple tests on boards we want maybe_legal() to pass...
    uint16_t const want_legal[] = {
        0b00000'00010'00110,
        0b00111'01111'01111,
    };
    for (uint16_t board : want_legal) {
        if (!maybe_legal(board)) {
            print_board(board);
            return 1;
        }
    }

    // ...then more tests on boards we want maybe_legal() to fail.
    uint16_t const want_not_legal[] = {
        0b00000'00000'00000,
        0b11111'11111'11111,
    };
    for (uint16_t board : want_not_legal) {
        if (maybe_legal(board)) {
            print_board(board);
            return 1;
        }
    }

    // Print and count all maybe-legal boards.
    int n = 0;
    for (uint16_t board = 0b00000'00000'00000; board <= 0b11111'11111'11111; board++) {
        if (maybe_legal(board)) {
            print_board(board);
            n += 1;
        }
    }
    printf("%d maybe-legal boards\n", n);

    return 0;
}
