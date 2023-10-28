#include "s21_cat.h"

int main(int argc, char **argv) {
  FILE *file = NULL;
  char **keys = malloc(sizeof(char *) * argc + sizeof(char) * argc * M);
  for (int i = 0; i < argc; i++) {
    keys[i] = (char *)(keys + argc) + M * i;
  }

  char **files = malloc(sizeof(char *) * argc + sizeof(char) * argc * M);
  for (int i = 0; i < argc; i++) {
    files[i] = (char *)(files + argc) + M * i;
  }

  handlingArguments(argc, argv, keys, files);

  file_out(file, keys, files);

  free(keys);
  free(files);
  return 0;
}

void str_manipulate(char **keys, char *str, char *str_prev, int *cnt,
                    int *cnt_s) {
  int i = 0;
  int s_flag = 0;
  int n_flag = 0;

  while (strcmp(keys[i], "")) {
    switch (keys[i][0]) {
      case 'e':
        if (strlen(str) != 0 && str[strlen(str) - 1] == '\n') {
          int l = strlen(str);
          str[l - 1] = '$';
          str[l] = '\n';
          str[l + 1] = '\0';
          //                    printf("%s", str);
        }
        break;
      case 'n':
        n_flag = 1;
        break;
      case 'b':
        if ((str_prev[strlen(str_prev) - 1] == '\n' ||
             strcmp(str_prev, "") == 0) &&
            strcmp(str, "\n")) {
          printf("%6.d  ", *cnt);
          (*cnt)++;
        }
        break;
      case 's':
        s_flag = 1;
        break;
    }
    ++i;
  }

  if (strcmp(str, "\n") == 0) {
    (*cnt_s)++;
  } else {
    *cnt_s = 0;
  }

  if (s_flag && *cnt_s > 1 && strcmp(str, "\n") == 0) {
    str[0] = '\0';
  } else if (s_flag == 0 && n_flag) {
    if (str_prev[strlen(str_prev) - 1] == '\n' || strcmp(str_prev, "") == 0) {
      printf("%6.d  ", *cnt);
      (*cnt)++;
    }
  } else if (s_flag == 1 && n_flag) {
    if (str_prev[strlen(str_prev) - 1] == '\n' || strcmp(str_prev, "") == 0) {
      printf("%6.d  ", *cnt);
      (*cnt)++;
    }
  }
}

void file_out(FILE *file, char **keys, char **files) {
  int cnt = 1;
  int cnt_s = 0;
  int i = 0;
  char str_prev[SIZE_STR];
  while (strcmp(files[i], "")) {
    if ((file = fopen(files[i], "r")) == NULL) {
      printf("ERROR\n");
    } else {
      char str[SIZE_STR];
      while (fgets(str, SIZE_STR, file)) {
        str_manipulate(keys, str, str_prev, &cnt, &cnt_s);
        printf("%s", str);
        strcpy(str_prev, str);
      }
    }
    ++i;
  }
}

void handlingArguments(int argc, char **argv, char **keys, char **files) {
  int j = 0;
  int arr[256];

  for (int k = 0; k < 256; k++) {
    arr[k] = 0;
  }

  for (int k = 1; k < argc; k++) {
    if (strcmp(argv[k], "-b") == 0 ||
        strcmp(argv[k], "--number-nonblank") == 0) {
      arr['b'] = 1;
    } else if (strcmp(argv[k], "-e") == 0 || strcmp(argv[k], "-E") == 0) {
      arr['e'] = 1;
    } else if (strcmp(argv[k], "-n") == 0 || strcmp(argv[k], "--number") == 0) {
      arr['n'] = 1;
    } else if (strcmp(argv[k], "-s") == 0 ||
               strcmp(argv[k], "--squeeze-blank") == 0) {
      arr['s'] = 1;
    } else if (strcmp(argv[k], "-t") == 0 || strcmp(argv[k], "-T") == 0) {
      arr['t'] = 1;
    } else {
      strcpy(files[j++], argv[k]);
    }
  }

  if (arr['b'] == 1) {
    arr['n'] = 0;
  }
  int i = 0;
  for (int k = 0; k < 256; k++) {
    if (arr[k]) {
      char s[2];
      s[0] = k;
      strcpy(keys[i++], s);
    }
  }
}