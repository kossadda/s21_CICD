#ifndef CATFLAGS_H
#define CATFLAGS_H
#include <errno.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  bool numberNonBlank;
  bool markEnd;
  bool numberAll;
  bool squeeze;
  bool tab;
  bool printNonPrintable;
} Flag;

Flag CatReadFlags(int argc, char *argv[]);
void CatSetTable(const char *table[static 256]);
void cat(int argc, char *argv[], Flag flags, const char *table[static 256]);
char cat_flag(FILE *file, Flag flags, int *num, bool tempest, char last_symbol,
              const char *table[static 256]);
void cat_flag_e(const char *table[static 256]);
void cat_flag_t(const char *table[static 256]);
void cat_flag_v(const char *table[static 256]);

#endif
