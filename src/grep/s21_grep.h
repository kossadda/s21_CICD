#ifndef GREPFLAGS_H
#define GREPFLAGS_H

#define _GNU_SOURCE

#include <errno.h>
#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  bool shablon;
  bool registr;
  bool invertor;
  bool sum_sovp;
  bool only_filename;
  bool num_sovp;
  bool no_filename;
  bool no_error;
  bool regular;
  bool search_only;
} Flag;

void preparcer(int argc, char *argv[], char **reg_name, char ***massive,
               int *num_mas);
Flag grepReadFlags(int argc, char *argv[]);
void parcer(int argc, char *argv[], char ***filename, int *num_file,
            char ***massive, int *num_mas, char *reg_name, Flag flags);
void grep_file(char **filename, int num_file, int file_now, char **massive,
               char *reg_name, int num_mas, FILE *file, Flag flags);
ssize_t getline(char **lineptr, size_t *n, FILE *stream);
void grep_flags(char *c, char **massive, int num_mas, char **filename,
                char *reg_name, int file_now, int num_file, int numeration,
                int *sum, int end, FILE *file, Flag flags);
int grep_search(char *c, char *search, int c_flag, char **filename,
                int file_now, int num_file, int numeration, int end, FILE *file,
                regex_t preg, regmatch_t match, Flag flags);
void grep_print(char *c, char **filename, int file_now, int num_file,
                int numeration, int end, regmatch_t match, FILE *file,
                Flag flags);
void moreFile(char **filename, int file_now, int num_file, Flag flags);
void free_memory(char ***filename, int num_file, char ***massive, int num_mas,
                 char **reg_name);

#endif