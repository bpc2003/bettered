#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bled.h"

char **readfile(char *filename)
{
	FILE *fp = fopen(filename, "r");
	char **buf = (char **)calloc(BUFSIZ, sizeof(char *));

	if (filename == NULL)
		return buf;

	if (fp == NULL) {
		perror(filename);
		return buf;
	}

	char *str = NULL;
	size_t size = 0;
	ssize_t read, tot = 0;

	for (int i = 0; (read = getline(&str, &size, fp)) > 0; ++i) {
		buf[i] = strdup(str);
		if (i > BUFSIZ)
			buf = realloc(buf, sizeof(char *) * sizeof(buf));
		tot += read;
	}
	free(str);

	fclose(fp);
	printf("%d\n", tot);
	return buf;
}

void writefile(char *filename, char **buf)
{
	if (filename == NULL) {
		fprintf(stderr, "?\n");
		return;
	}
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
	char **buf = (char **)calloc(BUFSIZ, sizeof(char *));
	fseek(tmp, 0, SEEK_SET);

	char *str = NULL;
	size_t size = 0;
	for (int i = 0; getline(&str, &size, tmp) > 0 && strcmp(str, ".\n"); ++i) {
		buf[i] = strdup(str);
		if (i > BUFSIZ)
			buf =
			    (char **)realloc(buf, sizeof(char *) * sizeof(buf));
	}
	free(str);
	return buf;
}

void writetmp(FILE *tmp, char **buf)
{
	fseek(tmp, 0, SEEK_SET);
	int tot = 0;
	for (int i = 0; buf[i]; ++i)
		tot += fprintf(tmp, "%s", buf[i]);

	fseek(tmp, tot, SEEK_SET);
	fputs(".\n", tmp);
}
