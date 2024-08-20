#include "bed.h"

char **readfile(char *filename, int suppress)
{
	char **buf = calloc(BUFSIZ, sizeof(char *));
	if (filename == NULL)
		return buf;
	FILE *fp = fopen(filename, "r");
	if (fp == NULL) {
		perror(filename);
		return buf;
	}

	char *str = NULL;
	size_t size = 0;
	ssize_t read, tot = 0;
	for (int i = 0; (read = getline(&str, &size, fp)) > 0 && i < BUFSIZ;
	     ++i) {
		buf[i] = strdup(str);
		tot += read;
	}
	free(str);
	fclose(fp);
	if (!suppress)
		printf("%d\n", tot);
	return buf;
}

void writefile(char *filename, char **buf, int suppress)
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
	if (!suppress)
		printf("%d\n", tot);
}

void appendfile(char *filename, char **buf, int suppress)
{
	if (filename == NULL) {
		fprintf(stderr, "?\n");
		return;
	}
	FILE *fp = fopen(filename, "a");
	ssize_t tot = 0;

	if (fp == NULL) {
		perror(filename);
		return;
	}

	for (int i = 0; buf[i]; ++i)
		tot += fprintf(fp, "%s", buf[i]);
	fclose(fp);
	if (!suppress)
		printf("%d\n", tot);
}

char **readtmp(FILE *tmp)
{
	char **buf = calloc(BUFSIZ, sizeof(char *));
	fseek(tmp, 0, SEEK_SET);

	char *str = NULL;
	size_t size = 0;
	for (int i = 0;
	     getline(&str, &size, tmp) > 0 && strcmp(str, ".\n") && i < BUFSIZ;
	     ++i) {
		buf[i] = strdup(str);
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
