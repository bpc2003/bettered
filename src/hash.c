#include "hash.h"

unsigned long hash(char **buf)
{
  unsigned long hash = 5381;
  for(int i = 0; buf[i]; i++) {
    for(int j = 0; buf[i][j]; j++)
      hash = ((hash << 5) + hash) + buf[i][j];
  }
  return hash;
}
