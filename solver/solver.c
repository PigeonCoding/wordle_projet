#include "../wordle/wordle.c"
#include <stdio.h>
#include <string.h>

char pool[MAX_WORDS][WORD_LENGTH];
int pool_count = 0;

void simulate_feedback(const char *guess, const char *target, attempt_response out[5]){
    char temp_guess[WORD_LENGTH];
    memcpy(temp_guess, guess, WORD_LENGTH);

    for (int i = 0; i < 5; i++) {
        int in_target = 0, in_guess = 0;

        for (int j = 0; j < 5; j++)
            if (target[j] == guess[i]) in_target++;

        for (int j = 0; j < 5; j++)
            if (temp_guess[j] == guess[i]) in_guess++;

        while (in_guess > in_target) {
            for (int j = 4; j >= 0; j--) {
                if (temp_guess[j] == guess[i]) {
                    temp_guess[j] = ' ';
                    in_guess--;
                    break;
                }
            }
        }
    }
    for (int i = 0; i < 5; i++) {
        if (temp_guess[i] == target[i]) {
            out[i].state = GOOD;
        } else if (strchr(target, temp_guess[i])) {
            out[i].state = EXISTS;
        } else {
            out[i].state = NUH;
        }
        out[i].letter = guess[i];
    }

}

int match(const char *cand, const char *guess) {
    attempt_response sim[5];
    simulate_feedback(guess, cand, sim);

    for (int i = 0; i < 5; i++)
        if (sim[i].state != all[i].state)
            return 0;

    return 1;
}

void filter_pool(const char *guess) {
    int w = 0;
    for (int i = 0; i < pool_count; i++) {
        if (match(pool[i], guess)) {
            memcpy(pool[w], pool[i], WORD_LENGTH);
            w++;
        }
    }
    pool_count = w;
}

int pattern_code(attempt_response fb[5]) {
    int c = 0;
    for (int i = 0; i < 5; i++) {
        c = c * 3 + fb[i].state;
    }
    return c;
}

int entropy_score(const char *g) {
    int counts[243] = {0};
    attempt_response sim[5];

    for (int i = 0; i < pool_count; i++) {
        simulate_feedback(g, pool[i], sim);
        int code = pattern_code(sim);
        counts[code]++;
    }

    int score = 0;
    for (int i = 0; i < 243; i++) {
        if (counts[i] > 0) {
            score += counts[i] * counts[i];
        }
    }

    return score;
}

void best_entropy_guess(char *out) {
    int best = 2069676967;
    int idx = 0;

    for (int i = 0; i < pool_count; i++) {
        int s = entropy_score(pool[i]);
        if (s < best) {
            best = s;
            idx = i;
        }
    }

    memcpy(out, pool[idx], WORD_LENGTH);
}

int main() {

    if (init_wordle()) return 1;

    for (int i = 0; i < wordCount; i++)
        memcpy(pool[i], words[i], WORD_LENGTH);

    pool_count = wordCount;
  printf("|=============================|\n");
  printf("|     SOLVER BY TEAM FLAN     |\n");
  printf("|=============================|\n");
  printf("| MY first guess = brain      |\n");

    for (int turn = 1; turn <= 6; turn++) {

        char guess_local[WORD_LENGTH];

        if (turn == 1) {
            memcpy(guess_local, "brain", 6);
        } else {
            best_entropy_guess(guess_local);
        }
        printf("|=============================|\n");
        printf("  Turn %d guess: %s        \n", turn, guess_local);
        
        memcpy(guess, guess_local, WORD_LENGTH);

        int r = check_word();
        if (r == 1) {
            printf("|=============================|\n");
            printf("|    SOLVED word was %s    |\n",target);
            printf("|=============================|\n");
            return 0;
        }

        filter_pool(guess_local);
        printf("   %d candidates left      \n", pool_count);
        printf("|=============================|\n");
    }
    printf("|=============================|\n");
    printf("|  LOSER word was was %s  |\n", target);
    printf("|=============================|\n");
    return 0;
}