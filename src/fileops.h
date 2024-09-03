#ifndef FILEOPS_H
#define FILEOPS_H

#include <stdio.h>

char **readfile(char *, int);
char **readtmp(FILE *);
void writefile(char *, char **, int);
void writetmp(FILE *, char **);
void appendfile(char *, char **, int);

#endif
