#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include "bled.h"

int *find(char **buf, char *pat, int *len)
{
  int val, pos;
  int *lines = (int *)calloc(1, sizeof(int));
  if (lines == NULL) {
    perror("Err");
    return NULL;
  }

  regex_t re;

  val = regcomp(&re, pat, 0);
  if (val) {
    perror(pat);
    return NULL;
  }
  pos = 0;
  for (int i = 0; buf[i]; ++i) {
    val = regexec(&re, buf[i], 0, NULL, 0);
    if (val == REG_NOMATCH)
      continue;
    else if (val == 0) {
      lines = (int *)realloc(lines, (++pos) * sizeof(int));
      lines[pos - 1] = i+1;
    }
  }

  *len = pos;
  regfree(&re);
  return lines;
}
