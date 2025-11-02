 #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_WORDS 6969
#define WORD_LENGTH 6 

// Colors I got from someone on github
#define GREEN   "\x1b[42m\x1b[30m" // green background I think?
#define YELLOW  "\x1b[43m\x1b[30m" // yellow background?
#define GRAY    "\x1b[47m\x1b[30m" // gray background?
#define RESET   "\x1b[0m"

// importing the file
int Dictionary(char words[MAX_WORDS][WORD_LENGTH], const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: could not open %s\n", filename);
        return 0;
    }

    int count = 0;
    while (count < MAX_WORDS && fscanf(file, "%5s", words[count]) == 1)
        count++;

    fclose(file);
    return count;
}

// feedback
void feedback(const char *guess, const char *target) {
    for (int i = 0; i < 5; i++) {
        if (guess[i] == target[i])
            printf(GREEN " %c " RESET, guess[i]);
        else if (strchr(target, guess[i]))
            printf(YELLOW " %c " RESET, guess[i]);
        else
            printf(GRAY " %c " RESET, guess[i]);
    }
    printf("\n");
}

int main() {
    char words[MAX_WORDS][WORD_LENGTH];
    int wordCount = Dictionary(words, "dictionary.txt");
    if (wordCount == 0) return 1;

    srand(time(NULL));
    const char *target = words[rand() % wordCount];

    char guess[WORD_LENGTH];
    int attempts = 6;

    printf("=====================================\n");
    printf("             WORDLE BY FLAN        \n");
    printf("=====================================\n");
    printf("Guess the 5-letter word. You have %d attempts.\n", attempts);

    while (attempts > 0) {
        printf("\nEnter your guess: ");
        scanf("%5s", guess);

        // Convert guess to lowercase from guy on github(using ASCII was smart)
        for (int i = 0; i < 5; i++)
            if (guess[i] >= 'A' && guess[i] <= 'Z')
                guess[i] += 32;

        if (strlen(guess) != 5) {
            printf("Please enter exactly 5 letters.\n");
            continue;
        }

        feedback(guess, target);

        if (strcmp(guess, target) == 0) {
            printf("\nCorrect! You found the word!\n");
            return 0;
        }

        attempts--;
        printf("Attempts left: %d\n", attempts);
    }

    printf("\nOut of attempts! The word was: %s\n", target);
    return 0;
}

