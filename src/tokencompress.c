#define _GNU_SOURCE
#include "tokencompress.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int suffixCompare(const void *a, const void *b, void *str) {
  return strcmp((char *)str + *(int *)a, (char *)str + *(int *)b);
}

// Function to build suffix array
void buildSuffixArray(char *s, int len, int *suffixArr) {
  for (int i = 0; i < len; i++) {
    suffixArr[i] = i;
  }
  qsort_r(suffixArr, len, sizeof(int), suffixCompare, s);
}

// Function to build LCP array
void buildLCPArray(char *s, int len, int *suffixArr, int *lcp) {
  int *rank = (int *)malloc(len * sizeof(int));
  for (int i = 0; i < len; i++) {
    rank[suffixArr[i]] = i;
  }
  int h = 0;
  for (int i = 0; i < len; i++) {
    if (rank[i] > 0) {
      int j = suffixArr[rank[i] - 1];
      while (i + h < len && j + h < len && s[i + h] == s[j + h]) {
        h++;
      }
      lcp[rank[i]] = h;
      if (h > 0) {
        h--;
      }
    }
  }
  free(rank);
}

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

RepeatPair FindBestRepeatPair(int *suffixArr, int *lcp, int len) {
  RepeatPair bestPair = {0, 0, 0};
  for (int i = 1; i < len; i++) {
    if (lcp[i] > bestPair.length) {
      bestPair.start = suffixArr[i];
      bestPair.length = lcp[i];
      bestPair.compressProfit = lcp[i] - 1;
    }
  }
  return bestPair;
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
