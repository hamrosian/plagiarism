#include "tokencompress.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
char *concat(const char *s1, const char *s2) {
  char *result = malloc(strlen(s1) + strlen(s2) + 1);
  if (result) {
    strcpy(result, s1);
    strcat(result, s2);
    return result;
  } else {
    return "";
  }
}

uint64_t kolmogorov_complexity(char *input) {
  int result = TokenCompress(input, strlen(input), "./.tmp/compressed.txt");
  printf("%d\n", result);
  return result;
#include <stdlib.h>
}

int main(int argc, char *argv[]) {

  if (argc != 3) {
    printf("Usage: %s [file 1] [file 2]", argv[0]);
    return 1;
  }

  FILE *file_a = fopen(argv[1], "r");
  char *a = 0;

  if (file_a) {
    fseek(file_a, 0, SEEK_END);
    uint64_t length = ftell(file_a);
    fseek(file_a, 0, SEEK_SET);
    a = malloc(length);
    if (a) {
      fread(a, 1, length, file_a);
    }
    fclose(file_a);
  }

  FILE *file_b = fopen(argv[2], "r");
  char *b = 0;

  if (file_b) {
    fseek(file_b, 0, SEEK_END);
    uint64_t length = ftell(file_b);
    fseek(file_b, 0, SEEK_SET);
    b = malloc(length);
    if (b) {
      fread(b, 1, length, file_b);
    }
    fclose(file_b);
  }

  if (a && b) {
    char *c = concat(a, b);

    double c_a = kolmogorov_complexity(a);
    double c_b = kolmogorov_complexity(b);
    double c_c = kolmogorov_complexity(c);

    double NCD = (c_c - fmin(c_a, c_b)) / (fmax(c_a, c_b));

    printf("The similarity of %s and %s is approximately %lf%%\n", argv[1],
           argv[2], ((1 - NCD) * 100));
    return 0;
  }
  return 1;
}
