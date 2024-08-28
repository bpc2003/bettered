#ifndef BETTERED_H
#define BETTERED_H
#include "bufops.h"
#include "fileops.h"
#include "token.h"
// TODO: Put each set of operations in their own header file

// regex
int *find(char **, char *, int *);
void substitute(char **, int, int, char *, char *);

// hash
unsigned long hash(char **);
#endif
