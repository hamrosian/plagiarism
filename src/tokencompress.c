#include "tokencompress.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

RepeatPair FindRepeatPair(char *s, int i, int len) {
  RepeatPair p = {0, 0, 0};
  for (int j = i + 1; j < len; j++) {
    int k = 0;
    while (s[i + k] == s[j + k] && (i + k) < len && (j + k) < len) {
      k++;
    }
    if (k > p.length) {
      p.start = j;
      p.length = k;
      p.compressProfit = k - 1; // Simplified profit calculation
    }
  }
  return p;
}

void EncodeRepeatPair(RepeatPair p, FILE *compFile) {
  fputc(p.start, compFile);
  fputc(p.length, compFile);
  fputc(0, compFile);
}

void AppendCharToBuffer(char c, char *buffer, int *bufLen) {
  buffer[*bufLen] = c;
  (*bufLen)++;
}

void EncodeLiteralZone(char *buffer, int bufLen, FILE *compFile) {
  for (int i = 0; i < bufLen; i++) {
    fputc(buffer[i], compFile);
  }
}

int TokenCompress(char *s, int len, const char *compFileName) {
  int i = 0;
  char buffer[1024];
  int bufLen = 0;
  FILE *compFile = fopen(compFileName, "w");
  if (compFile == NULL) {
    fprintf(stderr, "Error opening file.\n");
    return -1;
  }

  while (i < len) {
    RepeatPair p = FindRepeatPair(s, i, len);
    if (p.compressProfit > 0) {
      EncodeRepeatPair(p, compFile);
      i += p.length;
    } else {
      AppendCharToBuffer(s[i], buffer, &bufLen);
      i++;
    }
  }
  EncodeLiteralZone(buffer, bufLen, compFile);

  fclose(compFile);

  FILE *compFileSize = fopen(compFileName, "r");
  fseek(compFileSize, 0, SEEK_END);
  int compSize = ftell(compFileSize);
  fclose(compFileSize);

  return compSize;
}
