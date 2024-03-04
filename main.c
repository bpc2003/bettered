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

  int start = 1;
  int end = END;
  while(strcmp(cmd, "q\n")) {
    fgets(cmd, 100, stdin);
   
    int i = 0;

    // I'll add parsing later
    while(!isalpha(cmd[i])) {
      if(isdigit(cmd[i])){
        for(start = 0; isdigit(cmd[i]); i++)
          start = 10 * start + (cmd[i] - '0');
        if(cmd[i] == ',')
          i++;
        if(cmd[i] == '$') {
          end = END;
          i++;
        } else
          for(end = 0; isdigit(cmd[i]); i++)
            end = 10 * end + (cmd[i] - '0');
      } else if(cmd[i] == ',') {
        start = 1;
        end = END;
        i++;
      }
    }
   
    switch(cmd[i]) {
      case 'p':
      case 'n':
        printbuf(buf, cmd[i] == 'n', start, end);
        break;
      case 'a':
      case 'i':
      case 'c':
        insertbuf(buf, cmd[i] == 'c', cmd[i] == 'i');
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
  return 0;
}
