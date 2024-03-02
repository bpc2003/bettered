#include <stdio.h>
#include "bled.h"

int main(int argc, char **argv) {
  char *filename = argv[1];
  
  char **buf = readfile(filename);
  if(buf == NULL)
    return 1;

  for(int i = 0; buf[i]; ++i)
    printf("%s", buf[i]);
}
