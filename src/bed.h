#include <stdio.h>

#define END -1
#define get_len(buf, len) for(len = 0; buf[len]; ++len) ;

// file operations
char **readfile(char *, int);
char **readtmp(FILE *);
void writefile(char *, char **, int);
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

// hash
unsigned long hash(char **);
