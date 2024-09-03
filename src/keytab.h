#ifndef KEYTAB_H
#define KEYTAB_H

struct tab {
  char key;
  int *lines;
};

extern struct tab keytab[26];

int *getkey(char);
void addkey(char, int *);

#endif
