#ifndef S21_CAT_H
#define S21_CAT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE_STR 10000
#define M 256

void file_out(FILE *, char **, char **);

void handlingArguments(int, char **, char **, char **);

void str_manipulate(char **, char *, char *, int *, int *);

#endif  // S21_CAT_H