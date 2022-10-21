#include <stdint.h>
#include <stdio.h>

static void print_board(uint16_t board) {
    for (int i = 15; i --> 0;) {
        printf("%s%s", (board & (1<<i)) ? "🟩" : "⬛"
                     , (i % 5 == 0) ? "\n" : "");
    }
    printf("🟩🟩🟩🟩🟩\n");
}

static bool congruent_to_3_mod_4(uint16_t board) {
    switch (__builtin_popcount(board)) {
        case  3:
        case  7:
        case 11: return true;
    }
    return false;
}

static bool maybe_legal(uint16_t board) {
    return congruent_to_3_mod_4(board);
}

int main(void) {
    uint16_t const boards[] = {
        0b00000'00010'00110,
        0b00111'01111'01111,
    };

    for (uint16_t board : boards) {
        printf("%04x %s\n", board, maybe_legal(board) ? "ok" : "NO");
        print_board(board);
        printf("\n");
    }

    return 0;
}
