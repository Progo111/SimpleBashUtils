#ifndef S21_GREP_H
#define S21_GREP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "regex.h"
#include "unistd.h"

#define SIZE_STR 10000
#define M 256

void handlingArguments(int argc, char **argv, char **templates,
                       int *count_templates, char **files, int *count_files);

void outputOneFile(char **templates, int count_templates, char **files,
                   FILE *file, int *flags);

void outputSeveralFile(char **templates, int count_templates, char **files,
                       int count_files, FILE *file, int *flags);

void allTemplate(int argc, char **argv, char **templates, int *count_templates,
                 int *flags);

int str_compare_one_file(regex_t *preg, int preg_cnt, int *flags, FILE *file);

int str_compare_several_file(regex_t *preg, int preg_cnt, int *flags,
                             FILE *file, char *file_name);

#endif  // S21_GREP_H