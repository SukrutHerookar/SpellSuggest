#include "header.h"

#define BILLION 1000000000L;

int main(int argc, char *argv[]) {

  if (argc != 3) {
    printf("Usage: %s <dictionary.txt> <input.txt>\n", argv[0]);
    return 1;
  }

  FILE *fdict = fopen(argv[1], "r");
  if (!fdict) {
    perror(argv[1]);
    return 1;
  }
  printf("Dictionary file: %s...\n", argv[1]);

  FILE *ftext = fopen(argv[2], "r");
  if (!ftext) {
    perror(argv[2]);
    fclose(fdict);
    return 1;
  }
  printf("Check file: %s...\n", argv[2]);

  printf("++++++ Start ++++++\n");

  word_analyze(fdict, ftext);

  printf("++++++ Done +++++++\n");

  fclose(fdict);
  fclose(ftext);
  return 0;
}
