#include <stdio.h>
#include "bled.h"

void printbuf(char **buf) {
  for(int i = 0; buf[i]; ++i)
    printf("%s", buf[i]);
}
