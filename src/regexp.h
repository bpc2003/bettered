#ifndef REGEXP_H
#define REGEXP_H

#define END -1
#define get_len(buf, len) for(len = 0; buf[len]; ++len) ;

int *find(char **, char *, int *, int);
void substitute(char **, int, int, char *, char *);

#endif
