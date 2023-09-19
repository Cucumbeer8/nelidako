#ifndef CAT_H
#define CAT_H

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

// int number();
// int number_nonblank();
// int squeeze_blank();

typedef struct short_flags {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
  int E;
  int T;
} flg;

static struct option long_flags[] = {
    {"number-nonblank", no_argument, NULL, 'b'},
    {"number", no_argument, NULL, 'n'},
    {"squeeze-blank", no_argument, NULL, 's'},
    {NULL, no_argument, NULL, 0}};
#endif