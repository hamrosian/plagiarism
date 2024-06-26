#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int start;
  int length;
  int compressProfit;
} RepeatPair;

RepeatPair FindRepeatPair(char *s, int i, int len);
void EncodeRepeatPair(RepeatPair p, FILE *compFile);
void AppendCharToBuffer(char c, char *buffer, int *bufLen);
void EncodeLiteralZone(char *buffer, int bufLen, FILE *compFile);
int TokenCompress(char *s, int len, const char *compFileName);
