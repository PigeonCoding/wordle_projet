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
char guess_copy[WORD_LENGTH] = {0};

#define MAX_WORDS 6969
#define WORD_LENGTH 6

enum { NUH, GOOD, EXISTS };

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

int char_in_str(char c, const char *str) {
  int ret = 0;

  for (int cc = 0; cc < WORD_LENGTH - 1; cc++) {
    if (str[cc] == c)
      ret++;
  }

  return ret;
}

void removeNoccurences(char *str, char c, int num) {
  int len = strlen(str);

  for (int i = len - 1; i >= 0 && num > 0; i--) {
    if (str[i] == c) {
      num--;
      str[i] = ' ';
    }
  }
}

void dedup_str(char *str) {
  for (int i = 0; i < WORD_LENGTH - 1; i++) {
    if (char_in_str(str[i], target) < char_in_str(str[i], str)) {
      int rem = char_in_str(str[i], str) -
                char_in_str(str[i], target);

      removeNoccurences(str, str[i], rem);
    }
  }
}

void feedback(const char *guess, const char *target) {

  memcpy(guess_copy, guess, WORD_LENGTH);
  dedup_str(guess_copy);

  for (int i = 0; i < WORD_LENGTH - 1; i++) {
    if (guess_copy[i] == target[i]) {
      all[i].letter = guess[i];
      all[i].state = GOOD;
    } else if (strchr(target, guess_copy[i])) {
      all[i].letter = guess[i];
      all[i].state = EXISTS;
    } else {
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
    printf("|   must be 5 letter please   |\n");
    printf("|=============================|\n");
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
