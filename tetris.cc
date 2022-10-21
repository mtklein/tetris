#include <stdint.h>
#include <stdio.h>

static bool congruent_to_3_mod_4(uint16_t board) {
    return __builtin_popcount(board) % 4 == 3;
}

static bool no_all_green_rows(uint16_t board) {
    return (board & 0b00000'00000'11111) != 0b00000'00000'11111
        && (board & 0b00000'11111'00000) != 0b00000'11111'00000
        && (board & 0b11111'00000'00000) != 0b11111'00000'00000;
}

static bool strict(uint16_t board) {
    for (int i = 5; i < 10; i++) {
        if ( !(board & (1<<i)) && (board & (1<<(i+ 5))) ) { return false; }
    }
    for (int i = 0; i < 5; i++) {
        if ( !(board & (1<<i)) && (board & (1<<(i+ 5))) ) { return false; }
        if ( !(board & (1<<i)) && (board & (1<<(i+10))) ) { return false; }
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

int main(int argc, char *argv[]) {
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

    if (argc > 1) {
        (void)argv;

        int n = 0;
        for (int bits = 0; bits < (1<<15); bits++) {
            auto board = static_cast<uint16_t>(bits);

            if (maybe_legal(board)) {
                print_board(board);
                n += 1;
            }
        }
        printf("%d maybe-legal boards\n", n);
    }

    return 0;
}
