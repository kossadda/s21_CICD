#include "./s21_cat.h"

#include <string.h>

int main(int argc, char *argv[]) {
  Flag flags = CatReadFlags(argc, argv);
  const char *table[256];
  CatSetTable(table);
  if (flags.markEnd) {
    cat_flag_e(table);
  }
  if (flags.tab) {
    cat_flag_t(table);
  }
  if (flags.printNonPrintable) {
    cat_flag_v(table);
  }
  cat(argc, argv, flags, table);
}

Flag CatReadFlags(int argc, char *argv[]) {
  struct option longOptions[] = {{"number-nonblank", 0, NULL, 'b'},
                                 {"number", 0, NULL, 'n'},
                                 {"squeeze-blank", 0, NULL, 's'},
                                 {NULL, 0, NULL, 0}};
  int currentFlag = getopt_long(argc, argv, "bevEnstT", longOptions, NULL);
  Flag flags = {false, false, false, false, false, false};
  for (; currentFlag != -1;
       currentFlag = getopt_long(argc, argv, "bevEnstT", longOptions, NULL)) {
    if (currentFlag == 'b') flags.numberNonBlank = true;
    if (currentFlag == 'e') {
      flags.markEnd = true;
      flags.printNonPrintable = true;
    }
    if (currentFlag == 'v') flags.printNonPrintable = true;
    if (currentFlag == 'E') flags.markEnd = true;
    if (currentFlag == 'n') flags.numberAll = true;
    if (currentFlag == 's') flags.squeeze = true;
    if (currentFlag == 't') {
      flags.tab = true;
      flags.printNonPrintable = true;
    }
    if (currentFlag == 'T') flags.tab = true;
  }
  return flags;
}

void cat(int argc, char *argv[], Flag flags, const char *table[static 256]) {
  int num = 0;
  bool tempest = false;
  char last_symbol = 0;
  for (char **filename = &argv[0], **lastname = &argv[argc];
       filename != lastname; ++filename) {
    if (**filename == '-' || *filename == argv[0]) continue;
    FILE *file = fopen(*filename, "rb");
    if (errno) {
      fprintf(stderr, "%s: ", argv[0]);
      perror(*filename);
      continue;
    }
    last_symbol = cat_flag(file, flags, &num, tempest, last_symbol, table);
    fclose(file);
    tempest = true;
  }
}

char cat_flag(FILE *file, Flag flags, int *num, bool tempest, char last_symbol,
              const char *table[static 256]) {
  int c = 0;
  bool sq = false, temp = false;
  int last = '\n';
  while (fread(&c, 1, 1, file) > 0) {
    if (last == '\n') {
      if (flags.squeeze && c == '\n') {
        if (sq) continue;
        sq = true;
        if (flags.markEnd && !temp && tempest && !sq) printf("%c", '$');
      } else
        sq = false;
      if (flags.numberAll && !flags.numberNonBlank) {
        if (!tempest) printf("%6i\t", ++*num);
        if (tempest && !temp && c == '\n' && last_symbol == '\n')
          printf("%6i\t", ++*num);
        if (tempest && temp) printf("%6i\t", ++*num);
      } else if (flags.numberNonBlank && c != '\n') {
        if (!tempest) printf("%6i\t", ++*num);
        if (tempest && temp) printf("%6i\t", ++*num);
      }
    }
    if (c == '\0' && !flags.printNonPrintable) printf("%c", c);
    printf("%s", table[c]);
    last = c;
    temp = true;
  }
  return last;
}

void cat_flag_e(const char *table[static 256]) { table['\n'] = "$\n"; }

void cat_flag_t(const char *table[static 256]) { table['\t'] = "^I"; }

void cat_flag_v(const char *table[static 256]) {
  const char *mini_1[] = {"^@", "^A", "^B", "^C", "^D", "^E", "^F", "^G", "^H"};
  const char *mini_2[] = {"^K", "^L", "^M", "^N",  "^O", "^P", "^Q",
                          "^R", "^S", "^T", "^U",  "^V", "^W", "^X",
                          "^Y", "^Z", "^[", "^\\", "^]", "^^", "^_"};
  const char *mini_3[] = {
      "^?",   "M-^@", "M-^A",  "M-^B", "M-^C", "M-^D", "M-^E", "M-^F", "M-^G",
      "M-^H", "M-^I", "M-^J",  "M-^K", "M-^L", "M-^M", "M-^N", "M-^O", "M-^P",
      "M-^Q", "M-^R", "M-^S",  "M-^T", "M-^U", "M-^V", "M-^W", "M-^X", "M-^Y",
      "M-^Z", "M-^[", "M-^\\", "M-^]", "M-^^", "M-^_", "M- ",  "M-!",  "M-\"",
      "M-#",  "M-$",  "M-%",   "M-&",  "M-'",  "M-(",  "M-)",  "M-*",  "M-+",
      "M-,",  "M--",  "M-.",   "M-/",  "M-0",  "M-1",  "M-2",  "M-3",  "M-4",
      "M-5",  "M-6",  "M-7",   "M-8",  "M-9",  "M-:",  "M-;",  "M-<",  "M-=",
      "M->",  "M-?",  "M-@",   "M-A",  "M-B",  "M-C",  "M-D",  "M-E",  "M-F",
      "M-G",  "M-H",  "M-I",   "M-J",  "M-K",  "M-L",  "M-M",  "M-N",  "M-O",
      "M-P",  "M-Q",  "M-R",   "M-S",  "M-T",  "M-U",  "M-V",  "M-W",  "M-X",
      "M-Y",  "M-Z",  "M-[",   "M-\\", "M-]",  "M-^",  "M-_",  "M-`",  "M-a",
      "M-b",  "M-c",  "M-d",   "M-e",  "M-f",  "M-g",  "M-h",  "M-i",  "M-j",
      "M-k",  "M-l",  "M-m",   "M-n",  "M-o",  "M-p",  "M-q",  "M-r",  "M-s",
      "M-t",  "M-u",  "M-v",   "M-w",  "M-x",  "M-y",  "M-z",  "M-{",  "M-|",
      "M-}",  "M-~",  "M-^?"};
  memcpy(table, mini_1, sizeof mini_1);
  memcpy(&table[9 + 2], mini_2, sizeof mini_2);
  memcpy(&table['~' + 1], mini_3, sizeof mini_3);
}

void CatSetTable(const char *table[static 256]) {
  const char *rawTable[] = {
      "\00",  "\01",  "\02",  "\03",  "\04",  "\05",  "\06",  "\07",  "\b",
      "\t",   "\n",   "\v",   "\014", "\r",   "\016", "\017", "\020", "\021",
      "\022", "\023", "\024", "\025", "\026", "\027", "\030", "\031", "\032",
      "\033", "\034", "\035", "\036", "\037", " ",    "!",    "\"",   "#",
      "$",    "%",    "&",    "\'",   "(",    ")",    "*",    "+",    ",",
      "-",    ".",    "/",    "0",    "1",    "2",    "3",    "4",    "5",
      "6",    "7",    "8",    "9",    ":",    ";",    "<",    "=",    ">",
      "?",    "@",    "A",    "B",    "C",    "D",    "E",    "F",    "G",
      "H",    "I",    "J",    "K",    "L",    "M",    "N",    "O",    "P",
      "Q",    "R",    "S",    "T",    "U",    "V",    "W",    "X",    "Y",
      "Z",    "[",    "\\",   "]",    "^",    "_",    "`",    "a",    "b",
      "c",    "d",    "e",    "f",    "g",    "h",    "i",    "j",    "k",
      "l",    "m",    "n",    "o",    "p",    "q",    "r",    "s",    "t",
      "u",    "v",    "w",    "x",    "y",    "z",    "{",    "|",    "}",
      "~",    "\x7f", "\x80", "\x81", "\x82", "\x83", "\x84", "\x85", "\x86",
      "\x87", "\x88", "\x89", "\x8a", "\x8b", "\x8c", "\x8d", "\x8e", "\x8f",
      "\x90", "\x91", "\x92", "\x93", "\x94", "\x95", "\x96", "\x97", "\x98",
      "\x99", "\x9a", "\x9b", "\x9c", "\x9d", "\x9e", "\x9f", "\xa0", "\xa1",
      "\xa2", "\xa3", "\xa4", "\xa5", "\xa6", "\xa7", "\xa8", "\xa9", "\xaa",
      "\xab", "\xac", "\xad", "\xae", "\xaf", "\xb0", "\xb1", "\xb2", "\xb3",
      "\xb4", "\xb5", "\xb6", "\xb7", "\xb8", "\xb9", "\xba", "\xbb", "\xbc",
      "\xbd", "\xbe", "\xbf", "\xc0", "\xc1", "\xc2", "\xc3", "\xc4", "\xc5",
      "\xc6", "\xc7", "\xc8", "\xc9", "\xca", "\xcb", "\xcc", "\xcd", "\xce",
      "\xcf", "\xd0", "\xd1", "\xd2", "\xd3", "\xd4", "\xd5", "\xd6", "\xd7",
      "\xd8", "\xd9", "\xda", "\xdb", "\xdc", "\xdd", "\xde", "\xdf", "\xe0",
      "\xe1", "\xe2", "\xe3", "\xe4", "\xe5", "\xe6", "\xe7", "\xe8", "\xe9",
      "\xea", "\xeb", "\xec", "\xed", "\xee", "\xef", "\xf0", "\xf1", "\xf2",
      "\xf3", "\xf4", "\xf5", "\xf6", "\xf7", "\xf8", "\xf9", "\xfa", "\xfb",
      "\xfc", "\xfd", "\xfe", "\xff",
  };
  memcpy(table, rawTable, sizeof rawTable);
}
