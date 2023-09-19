#include <errno.h>
#include <getopt.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* -------------------- globals -------------------- */
const char *progname = "s21_cat";

static char const *optstring = "ETbnsetv";

static const struct option long_options[] = {
    {"number-nonblank", no_argument, 0, 'b'},
    {"number", no_argument, 0, 'n'},
    {"squeeze-blank", no_argument, 0, 's'},
    {0, 0, 0, 0}};

static bool NUM_NONBLANK = false;
static bool NUM_ALL = false;
static bool SQUEEZE_BLANK = false;
static bool SHOW_ENDL = false;
static bool SHOW_TAB = false;
static bool SHOW_NONPRINTING = false;

/* -------------------- func declarations -------------------- */
static void set_flag(char flag);

static void cat(FILE *file);

static void represent_char(unsigned char ch);

static void skip_endls(FILE *file);

/* -------------------- main -------------------- */
int main(int argc, char *argv[]) {
  int opt;
  int longopt_ind = 0;

  while ((opt = getopt_long(argc, argv, optstring, long_options,
                            &longopt_ind)) != -1) {
    if (opt == '?') {
      exit(EXIT_FAILURE);
    }
    set_flag(opt);
  }

  if (optind < argc) {
    for (int i = optind; i < argc; ++i) {
      FILE *file = fopen(argv[i], "r");
      if (file) {
        cat(file);
        fclose(file);
      } else {
        fprintf(stderr, "%s: %s: %s\n", progname, argv[i], strerror(errno));
      }
    }
  }
}

/* -------------------- func definitions -------------------- */
static void set_flag(char flag) {
  switch (flag) {
    case 'E':
      SHOW_ENDL = true;
      break;
    case 'T':
      SHOW_TAB = true;
      break;
    case 'b':
      NUM_NONBLANK = true;
      break;
    case 'n':
      NUM_ALL = true;
      break;
    case 's':
      SQUEEZE_BLANK = true;
      break;
    case 'e':
      SHOW_ENDL = true;
      SHOW_NONPRINTING = true;
      break;
    case 't':
      SHOW_TAB = true;
      SHOW_NONPRINTING = true;
      break;
    case 'v':
      SHOW_NONPRINTING = true;
      break;
    default:
      break;
  }
}

static void cat(FILE *file) {
  static size_t line_number = 1;

  int prev_char = '\n';
  int curr_char;

  while ((curr_char = fgetc(file)) != EOF) {
    unsigned char ch = curr_char;

    /*	print line number */
    if (prev_char == '\n') {
      bool print_line_number = false;

      if (NUM_NONBLANK) {
        if (curr_char != '\n' && curr_char != EOF) {
          print_line_number = true;
        }
      } else if (NUM_ALL) {
        print_line_number = true;
      }

      if (print_line_number) {
        printf("%6zu\t", line_number);
        line_number += 1;
      }
    }

    /*	print character */
    if (curr_char == '\n') {
      if (prev_char == '\n' && SQUEEZE_BLANK) {
        skip_endls(file);
      }
      if (SHOW_ENDL) {
        putchar('$');
      }
      putchar('\n');
    } else if (curr_char == '\t') {
      if (SHOW_TAB) {
        putchar('^');
        putchar('I');
      } else {
        putchar('\t');
      }
    } else {
      represent_char(ch);
    }

    prev_char = curr_char;
  }
}

static void represent_char(unsigned char ch) {
  if (SHOW_NONPRINTING) {
    if (ch >= 32) {
      if (ch < 127) {
        putchar((char)ch);
      } else if (ch == 127) {
        putchar('^');
        putchar('?');
      } else {
        putchar('M');
        putchar('-');
        if (ch >= 128 + 32) {
          if (ch < 128 + 127) {
            putchar((char)ch - 128);
          } else {
            putchar('^');
            putchar('?');
          }
        } else {
          putchar('^');
          putchar((char)ch - 128 + 64);
        }
      }
    } else {
      putchar('^');
      putchar((char)ch + 64);
    }
  } else {
    putchar((char)ch);
  }
}

void skip_endls(FILE *file) {
  int temp_char;
  while ((temp_char = fgetc(file)) == '\n') {
    ;
  }
  ungetc(temp_char, file);
}
