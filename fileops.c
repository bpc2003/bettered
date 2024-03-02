#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bled.h"

char **readfile(char *filename) {
  FILE *fp = fopen(filename, "r");
  if(fp == NULL) {
    perror(filename);
    return NULL;
  }

  char **buf = (char **) malloc(BUFSIZ * sizeof(char *));
  char *str = NULL;
  size_t size = 0;
  ssize_t read;

  for(int i  = 0; i < BUFSIZ && (read = getline(&str, &size, fp)) > 0; ++i)
    buf[i] = strdup(str);

  return buf;
}
