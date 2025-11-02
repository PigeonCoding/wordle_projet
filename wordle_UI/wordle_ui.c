#include "../wordle/wordle.c"
#include <stdio.h>

#define ATTEMPTS 6

#if 0
#define GREEN "\033[32m"  // green background I think?
#define YELLOW "\033[33m" // yellow background?
#define GRAY "\033[90m"   // gray background?
#define RESET "\033[0m"
#else
#define GREEN "\x1b[42m\x1b[30m"  // green background I think?
#define YELLOW "\x1b[43m\x1b[30m" // yellow background?
#define GRAY "\x1b[47m\x1b[30m"   // gray background?
#define RESET "\x1b[0m"
#endif

void petty_print() {
  printf("\033[A\033[K"); // Move up one line and clear it
  for (int i = 0; i < WORD_LENGTH - 1; i++) {

    switch (all[i].state) {
    case NUH:
      printf("|" GRAY "  %c  " RESET, all[i].letter);
      break;
    case GOOD:
      printf("|" GREEN "  %c  " RESET, all[i].letter);
      break;
    case EXISTS:
      printf("|" YELLOW "  %c  " RESET, all[i].letter);
      break;
    }
  }
  printf("|\n");
}

int main() {

  if (init_wordle())
    return 1;

  printf("|=============================|\n");
  printf("|     WORDLE BY TEAM FLAN     |\n");
  printf("|=============================|\n");
  printf("|  Guess the 5-letter word.   |\n"
         "|    You have %d attempts.     |\n",
         ATTEMPTS);
  printf("|=============================|\n");

  int a = ATTEMPTS;
  while (a > 0) {
    printf("enter your guess: ");
    scanf("%s", guess);

    switch (check_word()) {
    case -1:
      continue;
    case 1:
      petty_print();
      printf("|=============================|\n");
      return 0;
    case 0:
      petty_print();
      break;
    }

    a--;
    printf("|=============================|\n");
    printf("|    You have %d attempts.     |\n", a);
    printf("|=============================|\n");
  }
  
  printf("the corrrect word was %s\n", target);

  return 0;
}