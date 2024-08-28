#ifndef BUFOPS_H
#define BUFOPS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileops.h"

#define END -1
#define get_len(buf, len) for(len = 0; buf[len]; ++len) ;

void printlines(char **, int, int, int);
void insertlines(char **, int, int);
void changelines(char **, int, int);
void dellines(char **, int, int);
void movelines(char **, int, int, int, int);
void joinlines(char **, int, int);
void appendlines(char **, char *, int, int);
void undo(FILE *, char ***);

#endif
