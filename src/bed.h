#include <stdio.h>

#define END -1
#define get_len(buf, len) for(len = 0; buf[len]; ++len)

// file operations
char **readfile(char *);
void writefile(char *, char **);
char **readtmp(FILE *);
void writetmp(FILE *, char **);

// buffer operations
void printlines(char **, int, int, int);
void insertlines(char **, int, int);
void changelines(char **, int, int);
void dellines(char **, int, int);
void movelines(char **, int, int, int, int);
void undo(FILE *, char ***);

// regex
int *find(char **, char *, int *);
void substitute(char **, int, int, char *, char *);
