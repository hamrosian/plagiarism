#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

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
  // Input string to compress
  size_t inputLength = strlen(input) + 1; // Include the null terminator

  // Allocate memory for the compressed data
  // The maximum size of the compressed buffer is a bit more than the input size
  uint64_t compressedLength = compressBound(inputLength);
  Byte *compressedData = (Byte *)malloc(compressedLength);

  if (compressedData == NULL) {
    fprintf(stderr, "Failed to allocate memory for compressed data.\n");
  }

  // Compress the input string
  int result = compress(compressedData, &compressedLength,
                        (const unsigned char *)input, inputLength);

  if (result != Z_OK) {
    fprintf(stderr, "Failed to compress data: %d\n", result);
  }
  printf("%ld \n", compressedLength);
  free(compressedData);
  return compressedLength;
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
