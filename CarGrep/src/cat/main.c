#include "long_flags.h"

void parcer(int argc, char *argv[], flg *short_flags);
void reader(char *argv[], flg short_flags);

int main(int argc, char *argv[]) {
  flg short_flags = {};
  parcer(argc, argv, &short_flags);

  while (optind < argc) {
    reader(argv, short_flags);
    optind++;
  }
}

void parcer(int argc, char *argv[], flg *short_flags) {
  int flg;
  int flag_index;
  while ((flg = getopt_long(argc, argv, "+benstvTE", long_flags,
                            &flag_index)) != -1) {
    switch (flg) {
      case 'b':
        short_flags->b = 1;
        break;
      case 'e':
        short_flags->e = 1;
        short_flags->v = 1;
        break;
      case 'n':
        short_flags->n = 1;
        break;
      case 's':
        short_flags->s = 1;
        break;
      case 't':
        short_flags->t = 1;
        short_flags->v = 1;
        break;
      case 'v':
        short_flags->v = 1;
        break;
      case 'T':
        short_flags->T = 1;
        break;
      case 'E':
        short_flags->E = 1;
        break;
      case '?':
        exit(1);
    }
  }
  if (short_flags->b) {
    short_flags->n = 0;
  }
}

void reader(char *argv[], flg short_flags) {
  FILE *file = fopen(argv[optind], "r");

  if (file) {
    int current;
    int empty_str = 0;
    int order = 0;
    int check = '\n';
    while ((current = getc(file)) != EOF) {
      if ((short_flags.s) && (current == '\n') && (empty_str == 2)) {
        continue;
      }
      if (check == '\n' &&
          ((short_flags.b && current != '\n') || short_flags.n)) {
        printf("%6d\t", ++order);
      }
      if ((short_flags.t || short_flags.T) && current == '\t') {
        printf("^");
        current = 'I';
      }
      if (current == '\n') {
        empty_str++;
        if ((short_flags.e || short_flags.E) && (current == '\n')) {
          printf("$");
        }
      } else {
        if (empty_str) {
          empty_str = 0;
        }
      }
      if (short_flags.v) {
        if (current > 127 && current < 160) printf("M-^");
        if ((current < 32 && current != '\n' && current != '\t') ||
            current == 127)
          printf("^");
        if ((current < 32 || (current > 126 && current < 160)) &&
            current != '\n' && current != '\t')
          current = current > 126 ? current - 128 + 64 : current + 64;
      }
      printf("%c", current);
      check = current;
    }
    fclose(file);
  } else
    fprintf(stderr, "No such file or directory");
}