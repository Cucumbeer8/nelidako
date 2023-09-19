#include "s21_grep.h"
#define buffer 99999
#define MIN(a, b) ((a) < (b) ? (a) : (b))

int main(int argc, char *argv[]) {
  struct flags flg_opt;
  int return_result = 0;
  char pattern[buffer] = {0};
  memset(&flg_opt, 0, sizeof(flg_opt));
  return_result = parsing(argc, argv, &flg_opt, pattern);
  if ((argc >= 3) && (return_result != 1)) {
    grep(flg_opt, argc, argv, pattern);
  } else {
    return_result = -1;
  }
  return return_result;
}

int parsing(int argc, char *argv[], struct flags *flg_opt, char *pattern) {
  int i = 0, result = 0;
  while ((i = getopt(argc, argv, "e:ivclnhsf:o")) != -1) {
    if (i == 'e') {
        flg_opt->e = 1;
        snprintf(pattern, buffer, "%s", optarg);
        } else if (i == 'i') {
        flg_opt->i = 1;
        } else if (i == 'v') {
        flg_opt->v = 1;
        } else if (i == 'c') {
        flg_opt->c = 1;
        } else if (i == 'l') {
        flg_opt->l = 1;
        } else if (i == 'n') {
        flg_opt->n = 1;
        } else if (i == 'h') {
        flg_opt->h = 1;
        } else if (i == 's') {
        flg_opt->s = 1;
        } else if (i == 'f') {
        flg_opt->f = 1;
        snprintf(pattern, buffer, "%s", optarg);
        } else if (i == 'o') {
        flg_opt->o = 1;
        } else {
        result = 1;
        }
  }
  return result;
}

void grep(struct flags flg_opt, int argc, char *argv[], char *temp) {
  char pattern_new[buffer] = {0};
  int cols = 1;

  if (!flg_opt.f && !flg_opt.e) {
    snprintf(pattern_new, buffer, "%s", argv[optind++]);
  } else if (flg_opt.f) {
    cols = flag_f(pattern_new);
  }

  if (!flg_opt.f && flg_opt.e) {
    snprintf(pattern_new, buffer, "%s", temp);
  }

  if (cols != -1) {
    for (int i = optind; i < argc; i++) {
      if (flg_opt.e) {
        if (flg_opt.f) {
          FILE *file = fopen(pattern_new, "r");
          if (file != NULL) {
            char pattern[buffer];
            regex_t regex;
            while (fgets(pattern, buffer, file)) {
              pattern[strcspn(pattern, "\n")] = '\0';
              if (regcomp(&regex, pattern, 0) != 0) {
                continue;
              }
              flag_e(argc, argv, argv[i], flg_opt);
              regfree(&regex);
              rewind(file);
            }
            fclose(file);
          }
        } else {
          flag_e(argc, argv, argv[i], flg_opt);
        }
      } else {
        regular(flg_opt, pattern_new, argv[i]);
      }
    }
  }
}

void flag_e(int argc, char *argv[], char *file, struct flags flg_opt) {
  FILE *f;
  f = fopen(file, "r");
  if (f == NULL && !flg_opt.s) {
    fprintf(stderr, "No such file or directory");
  } else {
    char **patterns = &argv[2];
    char line[buffer - 1] = {0};
    int pattern_count = argc - 3;
    while (fgets(line, sizeof(line) - 1, f)) {
      for (int i = 0; i < pattern_count; i++) {
        if (strstr(line, patterns[i]) != NULL) {
          printf("%s:", file);
          printf("%s", line);
          break;
        }
      }
    }
    fclose(f);
  }
}

int flag_f(char *pattern) {
  FILE *file_f = fopen("pattern.regex", "r");

  if (file_f == NULL) {
    return 1;
  }

  // char pattern[buffer];
  regex_t regex;
  char line[buffer];

  while (fgets(pattern, buffer, file_f)) {
    pattern[strcspn(pattern, "\n")] = '\0';

    if (regcomp(&regex, pattern, 0) != 0) {
      continue;
    }

    while (fgets(line, buffer - 1, stdin)) {
      line[strcspn(line, "\n")] = '\0';

      if (regexec(&regex, line, 0, NULL, 0) == 0) {
        printf("%s\n", line);
      }
    }

    regfree(&regex);
    rewind(stdin);
  }

  fclose(file_f);

  return 0;
}

void regular(struct flags flg_opt, char *pattern, char *file) {
  int addiction_i = (flg_opt.i) ? REG_ICASE | REG_EXTENDED : REG_EXTENDED;
  regex_t regular;

  FILE *f = fopen(file, "r");

  if (f == NULL && !flg_opt.s) {
  } else {
    regcomp(&regular, pattern, addiction_i);
    parse_file(flg_opt, f, regular, file);
    regfree(&regular);
    fclose(f);
  }
}

void parse_file(struct flags flg_opt, FILE *f, regex_t regular, char *file) {
  char line[buffer - 1] = {0};
  regmatch_t pmatch[1];
  int count_str = 1, count_equallys = 0;

  while (fgets(line, sizeof(line) - 1, f)) {
    int rewiew = regexec(&regular, line, 1, pmatch, 0);
    int match = 0;

    if (rewiew == 0 && !flg_opt.v) match = 1;
    if (rewiew == REG_NOMATCH && flg_opt.v) match = 1;

    if (match && !flg_opt.l && !flg_opt.c && flg_opt.n)
      printf("%d:", count_str);
    if (match && !flg_opt.l && !flg_opt.c && !flg_opt.o) printf("%s", line);

    if (flg_opt.o && match) {
      for (int i = pmatch[0].rm_so; i < pmatch[0].rm_eo; i++) {
        printf("%c", line[i]);
      }
      printf("\n");
    }
    count_equallys += match;
    count_str++;
  }

  if (flg_opt.l && count_equallys > 0) printf("%s\n", file);
  if (flg_opt.c && !flg_opt.l) printf("%d\n", count_equallys);
}
