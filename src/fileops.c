#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bled.h"

char **readfile(char *filename)
{
	FILE *fp = fopen(filename, "r");
	char **buf = (char **)malloc(BUFSIZ * sizeof(char *));

	if (fp == NULL) {
		perror(filename);
		return buf;
	}

	char *str = NULL;
	size_t size = 0;
	ssize_t read, tot = 0;

	for (int i = 0; i < BUFSIZ && (read = getline(&str, &size, fp)) > 0;
	     ++i) {
		buf[i] = strdup(str);
		tot += read;
	}

	fclose(fp);
	printf("%d\n", tot);
	return buf;
}

void writefile(char *filename, char **buf)
{
	FILE *fp = fopen(filename, "w");
	ssize_t tot = 0;

	if (fp == NULL) {
		perror(filename);
		return;
	}

	for (int i = 0; buf[i]; ++i)
		tot += fprintf(fp, "%s", buf[i]);

	fclose(fp);
	printf("%d\n", tot);
	return;
}

char **readtmp(FILE *tmp)
{
	char **buf = (char **)malloc(BUFSIZ * sizeof(char *));
	fseek(tmp, 0, SEEK_SET);

	char *str = NULL;
	size_t size = 0;
	ssize_t read;
	for (int i = 0; i < BUFSIZ && (read = getline(&str, &size, tmp)) > 0;
	     ++i)
		buf[i] = strdup(str);
	return buf;
}

void writetmp(FILE *tmp, char **buf)
{
	fseek(tmp, 0, SEEK_SET);
	int tot = 0;
	for (int i = 0; buf[i]; ++i)
		tot += fprintf(tmp, "%s", buf[i]);

	fseek(tmp, tot, SEEK_SET);
	fputs("\n", tmp);
}
