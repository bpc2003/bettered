#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "keytab.h"

static struct {
  char key;
  int *lines;
} keytab[26];

int *getkey(char key)
{
  int idx = key % 26;
  return keytab[idx].lines;
}

void addkey(char key, int lines[2])
{
  int idx = key % 26;
  if (keytab[idx].lines != NULL)
    free(keytab[idx].lines);

  keytab[idx].key = key;
  keytab[idx].lines = calloc(2, sizeof(int));
  memcpy(keytab[idx].lines, lines, 2 * sizeof(int));
}

void freekeytab(void)
{
  for (int i = 0; i < 26; ++i)
    if (keytab[i].lines != NULL)
      free(keytab[i].lines);
}
