#include <stdio.h>
#include <string.h>
#include "bled.h"

void printbuf(char **buf, int sl, int start, int end) {
  for(int i = start-1; buf[i]; ++i) {
    if(i == end)
      break;
    if(sl)
      printf("%d\t%s", i+1, buf[i]);
    else
      printf("%s", buf[i]);
  }
}

void insertbuf(char **buf, int ow, int ins) {
  char *tmp = NULL;
  size_t size = 0;
  int i = 0;

  while(buf[i])
    i++;

  getline(&tmp, &size, stdin);
  if(strcmp(tmp, ".\n"))
    buf[i] = strdup(tmp);

  while(1) {
    getline(&tmp, &size, stdin);
    if(!strcmp(tmp, ".\n"))
      break;
    buf[++i] = strdup(tmp);
  }
}
