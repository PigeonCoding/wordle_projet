#include <stdbool.h>
#include <stdio.h>

enum {
  NUH,
  GOOD,
  EXISTS,
};

typedef struct {
  char letter;
  int state;
} attempt_response;

attempt_response all[5] = {0};

bool check_word(const char *attempt) {
  printf("check_word not implemented yet\n");
  return false;
}
