#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "bled.h"

int main(int argc, char **argv) {
  if(argc < 2)
    return 1;

  char *filename = argv[1];
  char cmd[100] = "";
  
  char **buf = readfile(filename);

  while(strcmp(cmd, "q\n")) {
    fgets(cmd, 100, stdin);
    
    int i = 0;
    // I'll add parsing later
    while(!isalpha(cmd[i]))
      i++;
 
    switch(cmd[i]) {
      case 'p':
        printbuf(buf);
        break;
      case 'w':
        writefile(filename, buf);
        break;
      case 'q':
        break;
      default:
        fprintf(stderr, "?\n");
        break;
    }
  }
}
