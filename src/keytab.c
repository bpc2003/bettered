#include "keytab.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

static struct {
  char key;
  int *lines;
} keytab[26];

int *getkey(char key)
{
  if (!islower(key)) {
    fprintf(stderr, "?\n");
    return NULL;
  }

  int idx = key % 26;
  return keytab[idx].lines;
}

void addkey(char key, int lines[2])
{
  if (!islower(key)) {
    fprintf(stderr, "?\n");
    return;
  }

  int idx = key % 26;
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
