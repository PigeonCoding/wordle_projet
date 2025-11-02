#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_WORDS 6969
#define WORD_LENGTH 6

int wordCount = 0;
char words[MAX_WORDS][WORD_LENGTH] = {0};
const char *target = NULL;
char guess[WORD_LENGTH] = {0};

#define MAX_WORDS 6969
#define WORD_LENGTH 6

enum {
  NUH,
  GOOD,
  EXISTS
};

typedef struct {
  char letter;
  int state;
} attempt_response;

attempt_response all[WORD_LENGTH - 1] = {0};
int Dictionary(char words[MAX_WORDS][WORD_LENGTH], const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    printf("[ERROR]: could not open %s\n", filename);
    return 0;
  }

  int count = 0;
  while (count < MAX_WORDS && fscanf(file, "%5s", words[count]) == 1)
    count++;

  fclose(file);
  return count;
}

void feedback(const char *guess, const char *target) {
  for (int i = 0; i < WORD_LENGTH - 1; i++) {
    if (guess[i] == target[i]) {
      // printf(GREEN " %c " RESET, guess[i]);
      all[i].letter = guess[i];
      all[i].state = GOOD;
    } else if (strchr(target, guess[i])) {
      // printf(YELLOW " %c " RESET, guess[i]);
      all[i].letter = guess[i];
      all[i].state = EXISTS;
    } else {
      // printf(GRAY " %c " RESET, guess[i]);
      all[i].letter = guess[i];
      all[i].state = NUH;
    }
  }
}

#define TO_LOWER(c) ((c) >= 'A' && (c) <= 'Z' ? (c) + 'a' - 'A' : (c))

int init_wordle() {
  srand(time(NULL));
  wordCount = Dictionary(words, "dictionary.txt");
  if (wordCount == 0)
    return 1;

  target = words[rand() % wordCount];

  return 0;
}

int check_word() {

  if (strlen(guess) != 5) {
    printf("\033[A\033[K"); // Move up one line and clear it
    printf("|  must be 5 letter please   |\n");
    printf("|============================|\n");
    return -1;
  }

  for (int g = 0; g < sizeof(guess) - 1; g++) {
    guess[g] = TO_LOWER(guess[g]);
  }

  feedback(guess, target);

  if (strcmp(guess, target) == 0) {
    // printf("\nCorrect! You found the word!\n");
    return 1;
  }

  return 0;
}
