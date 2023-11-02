#include "s21_grep.h"

int main(int argc, char **argv) {
  FILE *file = NULL;
  char **templates = malloc(sizeof(char *) * M + sizeof(char) * M * M);
  int count_templates = 0;
  for (int i = 0; i < M; i++) {
    templates[i] = (char *)(templates + M) + M * i;
    strcpy(templates[i], "");
  }

  int flags[M];
  for (int i = 0; i < M; ++i) {
    flags[i] = 0;
  }
  allTemplate(argc, argv, templates, &count_templates, flags);

  char **files = malloc(sizeof(char *) * argc + sizeof(char) * argc * M);
  int count_files = 0;
  for (int i = 0; i < argc; i++) {
    files[i] = (char *)(files + argc) + M * i;
    strcpy(files[i], "");
  }

  handlingArguments(argc, argv, templates, &count_templates, files,
                    &count_files);

  if (count_files == 1) {
    outputOneFile(templates, count_templates, files, file, flags);
  } else {
    outputSeveralFile(templates, count_templates, files, count_files, file,
                      flags);
  }

  free(files);
  free(templates);
  return 0;
}

void handlingArguments(int argc, char **argv, char **templates,
                       int *count_templates, char **files, int *count_files) {
  int flag = 1;
  int j = 0;
  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] == '-') {
    } else if (flag && *count_templates == 0) {
      strcat(templates[(*count_templates)++], argv[i]);
      flag = 0;
    } else if (strcmp(argv[i - 1], "-e") != 0 && strcmp(argv[i - 1], "-f") &&
               *count_templates > 0) {
      strcpy(files[j++], argv[i]);
      *count_files = j;
    }
  }
}

int str_compare_one_file(regex_t *preg, int preg_cnt, int *flags, FILE *file) {
  int result = 0;
  int str_num = 1;
  char str[SIZE_STR] = "\0";
  if (flags['v'] == 0) {
    while (fgets(str, SIZE_STR, file)) {
      for (int i = 0; i < preg_cnt; ++i) {
        regmatch_t pm;
        if (regexec(&preg[i], str, 1, &pm, 0) == 0) {
          if (flags['l'] + flags['c'] == 0) {
            if (flags['n']) {
              printf("%d:", str_num);
            }
            if (flags['o'] == 0) {
              printf("%s", str);
              if (str[strlen(str) - 1] != '\n') {
                printf("\n");
              }
            } else {
              char substr[SIZE_STR] = "";
              strncpy(substr, str + pm.rm_so, pm.rm_eo - pm.rm_so);
              printf("%s", substr);
              if (substr[strlen(substr) - 1] != '\n') {
                printf("\n");
              }
            }
          }
          result++;
          break;
        }
      }
      str_num++;
    }
  } else {
    while (fgets(str, SIZE_STR, file)) {
      int flag = 1;
      for (int i = 0; i < preg_cnt; ++i) {
        regmatch_t pm;
        if (regexec(&preg[i], str, 0, &pm, 0) == 0) {
          flag = 0;
          break;
        }
      }
      if (flag) {
        if (flags['l'] + flags['c'] == 0 && flags['o'] == 0) {
          if (flags['n']) {
            printf("%d:", str_num);
          }
          printf("%s", str);
          if (str[strlen(str) - 1] != '\n') {
            printf("\n");
          }
        }
        result++;
      }
      str_num++;
    }
  }
  return result;
}

void outputOneFile(char **templates, int count_templates, char **files,
                   FILE *file, int *flags) {
  regex_t preg[count_templates];
  int regflags = REG_EXTENDED;
  if (flags['i']) {
    regflags = REG_ICASE;
  }

  for (int i = 0; i < count_templates; i++) {
    regcomp(&preg[i], templates[i], regflags);
  }

  if ((file = fopen(files[0], "r")) == NULL) {
    if (flags['s'] == 0) {
      printf("ERROR\n");
    }
  } else {
    int cnt_comare = str_compare_one_file(preg, count_templates, flags, file);
    if (flags['l'] && cnt_comare > 0) {
      printf("%s\n", files[0]);
    } else if (flags['c']) {
      printf("%d\n", cnt_comare);
    }
    fclose(file);
  }

  for (int i = 0; i < count_templates; i++) {
    regfree(&preg[i]);
  }
}

int str_compare_several_file(regex_t *preg, int preg_cnt, int *flags,
                             FILE *file, char *file_name) {
  int result = 0;
  int str_num = 1;
  char str[SIZE_STR];
  if (flags['v'] == 0) {
    while (fgets(str, SIZE_STR, file)) {
      for (int i = 0; i < preg_cnt; ++i) {
        regmatch_t pm;
        if (regexec(&preg[i], str, 1, &pm, 0) == 0) {
          if (flags['l'] + flags['c'] == 0) {
            if (flags['h'] == 0) {
              printf("%s:", file_name);
            }
            if (flags['n']) {
              printf("%d:", str_num);
            }
            if (flags['o'] == 0) {
              printf("%s", str);
              if (str[strlen(str) - 1] != '\n') {
                printf("\n");
              }
            } else {
              char substr[SIZE_STR] = "";
              strncpy(substr, str + pm.rm_so, pm.rm_eo - pm.rm_so);
              printf("%s", substr);
              if (substr[strlen(substr) - 1] != '\n') {
                printf("\n");
              }
            }
          }
          result++;
          break;
        }
      }
      str_num++;
    }
  } else {
    while (fgets(str, SIZE_STR, file)) {
      int flag = 1;
      for (int i = 0; i < preg_cnt; ++i) {
        regmatch_t pm;
        if (regexec(&preg[i], str, 0, &pm, 0) == 0) {
          flag = 0;
          break;
        }
      }
      if (flag) {
        if (flags['l'] + flags['c'] == 0 && flags['o'] == 0) {
          if (flags['h'] == 0) {
            printf("%s:", file_name);
          }
          if (flags['n']) {
            printf("%d:", str_num);
          }
          printf("%s", str);
          if (str[strlen(str) - 1] != '\n') {
            printf("\n");
          }
        }
        result++;
      }
      str_num++;
    }
  }
  return result;
}

void outputSeveralFile(char **templates, int count_templates, char **files,
                       int count_files, FILE *file, int *flags) {
  regex_t preg[count_templates];
  int regflags = REG_EXTENDED;
  if (flags['i']) {
    regflags = REG_ICASE;
  }

  for (int i = 0; i < count_templates; i++) {
    regcomp(&preg[i], templates[i], regflags);
  }

  for (int i = 0; i < count_files; i++) {
    if ((file = fopen(files[i], "r")) == NULL) {
      if (flags['s'] == 0) {
        printf("ERROR\n");
      }
    } else {
      int cnt_comare = str_compare_several_file(preg, count_templates, flags,
                                                file, files[i]);
      if (flags['l'] && cnt_comare > 0) {
        printf("%s\n", files[i]);
      } else if (flags['c']) {
        printf("%s:%d\n", files[i], cnt_comare);
      }
      fclose(file);
    }
  }
  for (int i = 0; i < count_templates; i++) {
    regfree(&preg[i]);
  }
}

void allTemplate(int argc, char **argv, char **templates, int *count_templates,
                 int *flags) {
  int rez = 0;
  while ((rez = getopt(argc, argv, "e:f:clivnhso")) != -1) {
    FILE *f;
    char str[SIZE_STR];
    switch (rez) {
      case 'e':
        strcpy(templates[(*count_templates)++], optarg);
        break;
      case 'f':
        if ((f = fopen(optarg, "r")) == NULL) {
          printf("ERROR\n");
          break;
        } else {
          while (fgets(str, SIZE_STR, f)) {
            if (str[strlen(str) - 1] == '\n') {
              str[strlen(str) - 1] = '\0';
            }
            strcpy(templates[(*count_templates)++], str);
          }
          fclose(f);
        }
        break;
      case 'i':
        flags['i'] = 1;
        break;
      case 'c':
        flags['c'] = 1;
        break;
      case 'v':
        flags['v'] = 1;
        break;
      case 'l':
        flags['l'] = 1;
        break;
      case 'n':
        flags['n'] = 1;
        break;
      case 'h':
        flags['h'] = 1;
        break;
      case 's':
        flags['s'] = 1;
        break;
      case 'o':
        flags['o'] = 1;
        break;
    }
  }
}