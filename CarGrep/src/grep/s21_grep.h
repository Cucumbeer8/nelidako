#ifndef CAT_H
#define CAT_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// int number();
// int number_nonblank();
// int squeeze_blank();

struct flags {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int o;
  int h;
  int s;
  int f;
};

int parsing(int argc, char *argv[], struct flags *flg_opt, char *pattern);
void grep(struct flags flg_opt, int argc, char *argv[], char *pattern_new);
void flag_e(int argc, char *argv[], char *file, struct flags flg_opt);
void regular(struct flags flg_opt, char *pattern, char *file);
void parse_file(struct flags flg_opt, FILE *fp, regex_t reg, char *file);
int flag_f(char *pattern);

#endif
