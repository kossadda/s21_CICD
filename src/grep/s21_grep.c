#include "s21_grep.h"

int main(int argc, char *argv[]) {
  char **filename = NULL;
  char **massive = NULL;
  char *reg_name = NULL;
  int num_file = 0;
  int num_mas = 0;
  FILE *temp = NULL;
  preparcer(argc, argv, &reg_name, &massive, &num_mas);
  Flag flags = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  flags = grepReadFlags(argc, argv);
  parcer(argc, argv, &filename, &num_file, &massive, &num_mas, reg_name, flags);
  for (int file_now = 0; file_now < num_file; file_now++) {
    FILE *file = fopen(filename[file_now], "rb");
    if (flags.regular) temp = fopen(reg_name, "rb");
    if (flags.regular && temp != NULL) fclose(temp);
    if (file == NULL || (temp == NULL && flags.regular)) {
      if (!flags.no_error || (flags.regular && temp == NULL)) {
        fprintf(stderr, "%s: ", argv[0]);
        if (flags.regular && temp == NULL)
          perror(reg_name);
        else if (!flags.regular)
          perror(filename[file_now]);
      }
      continue;
    }
    grep_file(filename, num_file, file_now, massive, reg_name, num_mas, file,
              flags);
    fclose(file);
  }
  free_memory(&filename, num_file, &massive, num_mas, &reg_name);
  return 0;
}

void preparcer(int argc, char *argv[], char **reg_name, char ***massive,
               int *num_mas) {
  for (int itter = 0; itter < argc; ++itter) {
    if (strstr(argv[itter], "-f") != NULL) {
      *reg_name = (char *)malloc((strlen(argv[itter + 1]) + 1) * sizeof(char));
      strcpy(*reg_name, argv[itter + 1]);
    }
    if (strstr(argv[itter], "-e") != NULL ||
        (argv[itter][0] == '-' &&
         argv[itter][strlen(argv[itter]) - 1] == 'e')) {
      char **new_massive =
          (char **)realloc(*massive, (*num_mas + 1) * sizeof(char *));
      if (new_massive != NULL) {
        *massive = new_massive;
        (*massive)[*num_mas] =
            (char *)malloc((strlen(argv[itter + 1]) + 1) * sizeof(char));
        strcpy((*massive)[(*num_mas)++], argv[itter + 1]);
      }
    }
  }
}

Flag grepReadFlags(int argc, char *argv[]) {
  int currentFlag = getopt(argc, argv, "e:ivclnhsf:o");
  Flag flags = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  for (; currentFlag != -1; currentFlag = getopt(argc, argv, "e:ivclnhsf:o")) {
    if (currentFlag == 'e') flags.shablon = true;
    if (currentFlag == 'i') flags.registr = true;
    if (currentFlag == 'v') flags.invertor = true;
    if (currentFlag == 'c') flags.sum_sovp = true;
    if (currentFlag == 'l') flags.only_filename = true;
    if (currentFlag == 'n') flags.num_sovp = true;
    if (currentFlag == 'h') flags.no_filename = true;
    if (currentFlag == 's') flags.no_error = true;
    if (currentFlag == 'f') flags.regular = true;
    if (currentFlag == 'o') flags.search_only = true;
  }
  return flags;
}

void parcer(int argc, char *argv[], char ***filename, int *num_file,
            char ***massive, int *num_mas, char *reg_name, Flag flags) {
  int shablon_check = 0;
  for (int itter = 0; itter < argc; ++itter) {
    if (*argv[itter] == '-' || argv[itter] == argv[0]) continue;
    if (flags.regular && strcmp(argv[itter], reg_name) == 0) continue;
    for (int m = 0; m < *num_mas; m++) {
      if (flags.shablon && strcmp(argv[itter], (*massive)[m]) == 0) {
        shablon_check++;
        break;
      }
    }
    if (shablon_check) {
      shablon_check = 0;
      continue;
    }
    if (!flags.shablon && !flags.regular && *num_mas == 0) {
      *massive = (char **)realloc(*massive, (*num_mas + 1) * sizeof(char *));
      (*massive)[*num_mas] =
          (char *)malloc((strlen(argv[itter]) + 1) * sizeof(char));
      strcpy((*massive)[(*num_mas)++], argv[itter]);
      continue;
    }
    char **new_filename =
        (char **)realloc(*filename, (*num_file + 1) * sizeof(char *));
    if (new_filename != NULL) {
      *filename = new_filename;
      (*filename)[*num_file] =
          (char *)malloc((strlen(argv[itter]) + 1) * sizeof(char));
      strcpy((*filename)[(*num_file)++], argv[itter]);
    }
  }
}

void grep_file(char **filename, int num_file, int file_now, char **massive,
               char *reg_name, int num_mas, FILE *file, Flag flags) {
  size_t bufsize = 0;
  ssize_t linel = 0;
  int sum = 0;
  int numeration = 0;
  int end = 0;
  fseek(file, 0, 2);
  end = ftell(file);
  fseek(file, 0, 0);
  char *c = NULL;
  while ((linel = getline(&c, &bufsize, file)) != -1) {
    numeration++;
    grep_flags(c, massive, num_mas, filename, reg_name, file_now, num_file,
               numeration, &sum, end, file, flags);
  }
  if (sum > 0 && flags.only_filename)
    moreFile(filename, file_now, num_file, flags);
  if (flags.sum_sovp && !flags.only_filename) {
    moreFile(filename, file_now, num_file, flags);
    printf("%d\n", sum);
  }
  free(c);
}

void grep_flags(char *c, char **massive, int num_mas, char **filename,
                char *reg_name, int file_now, int num_file, int numeration,
                int *sum, int end, FILE *file, Flag flags) {
  regex_t preg = {0};
  regmatch_t match = {0};
  size_t bfsz = 0;
  int trigger = 0;
  int c_flag = REG_NEWLINE;
  if (flags.registr) c_flag = REG_ICASE;
  if (flags.regular) {
    FILE *file1 = fopen(reg_name, "rb");
    if (file1 != NULL) {
      char *m = NULL;
      ssize_t lnl = 0;
      while ((lnl = getline(&m, &bfsz, file1)) != -1) {
        for (size_t i = 0; i < strlen(m); i++)
          if (m[i] == '\n') m[i] = '\0';
        if (grep_search(c, m, c_flag, filename, file_now, num_file, numeration,
                        end, file, preg, match, flags))
          trigger++;
      }
      free(m);
      fclose(file1);
    }
  }
  if (!flags.regular) {
    for (int i = 0; i < num_mas; i++) {
      if (grep_search(c, massive[i], c_flag, filename, file_now, num_file,
                      numeration, end, file, preg, match, flags))
        trigger = true;
    }
  }
  if (flags.regular && flags.shablon) {
    for (int i = 0; i < num_mas; i++) {
      if (grep_search(c, massive[i], c_flag, filename, file_now, num_file,
                      numeration, end, file, preg, match, flags))
        trigger = true;
    }
  }
  if ((trigger && !flags.invertor) || (!trigger && flags.invertor)) {
    if (!flags.sum_sovp && !flags.only_filename && !flags.search_only)
      grep_print(c, filename, file_now, num_file, numeration, end, match, file,
                 flags);
    ++(*sum);
  }
}

int grep_search(char *c, char *search, int c_flag, char **filename,
                int file_now, int num_file, int numeration, int end, FILE *file,
                regex_t preg, regmatch_t match, Flag flags) {
  int trig = false;
  char *temp = c;
  if (regcomp(&preg, search, c_flag)) {
    exit(1);
  }
  while (!regexec(&preg, temp, 1, &match, 0)) {
    trig++;
    if (flags.search_only && !flags.only_filename && !flags.invertor &&
        !flags.sum_sovp)
      grep_print(temp, filename, file_now, num_file, numeration, end, match,
                 file, flags);
    if (strlen(temp) == strlen(search))
      break;
    else
      temp += match.rm_eo + 1;
  }
  regfree(&preg);
  return trig;
}

void grep_print(char *c, char **filename, int file_now, int num_file,
                int numeration, int end, regmatch_t match, FILE *file,
                Flag flags) {
  moreFile(filename, file_now, num_file, flags);
  if (flags.num_sovp) printf("%d:", numeration);
  for (size_t i = 0; i < strlen(c); i++) {
    if (!flags.search_only)
      printf("%c", c[i]);
    else {
      if ((int)i >= match.rm_so && (int)i < match.rm_eo) printf("%c", c[i]);
    }
  }
  if (flags.search_only) printf("\n");
  if (ftell(file) == end && c[strlen(c) - 1] != '\n' && !flags.search_only)
    printf("\n");
}

void moreFile(char **filename, int file_now, int num_file, Flag flags) {
  if (flags.only_filename) printf("%s\n", filename[file_now]);
  if (num_file > 1 && !flags.only_filename && !flags.no_filename)
    printf("%s:", filename[file_now]);
}

void free_memory(char ***filename, int num_file, char ***massive, int num_mas,
                 char **reg_name) {
  if (*reg_name != NULL) {
    free(*reg_name);
    *reg_name = NULL;
  }
  for (int i = 0; i < num_file; i++) {
    if ((*filename)[i] != NULL) {
      free((*filename)[i]);
      (*filename)[i] = NULL;
    }
  }
  for (int i = 0; i < num_mas; i++) {
    if ((*massive)[i] != NULL) {
      free((*massive)[i]);
      (*massive)[i] = NULL;
    }
  }
  if (*filename != NULL) free(*filename);
  *filename = NULL;
  if (*massive != NULL) free(*massive);
  *massive = NULL;
}
