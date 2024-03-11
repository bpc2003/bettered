#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bled.h"

void printbuf(char **buf, int sl, int start, int end)
{
	for (int i = start - 1; buf[i]; ++i) {
		if (i == end)
			break;
		if (sl)
			printf("%d\t%s", i + 1, buf[i]);
		else
			printf("%s", buf[i]);
	}
}

void insertbuf(char **buf, int pos, int ow, int ins)
{
	char *tmp = NULL;
	size_t size = 0;
	char **tmpbuf = (char **)malloc(sizeof(char *));
	if (tmpbuf == NULL) {
		perror("Err");
		return;
	}

	int nl = 0;

	while (1) {
		getline(&tmp, &size, stdin);
		if (!strcmp(tmp, ".\n"))
			break;
		tmpbuf =
		    (char **)realloc(tmpbuf, sizeof(char *) * sizeof(tmpbuf));
		tmpbuf[nl++] = strdup(tmp);
	}
	free(tmp);

	int len = 0;
	for (; buf[len]; len++) ;
	if (pos == END || pos > len)
		pos = len;

	if (!ins && !ow) {
		for (int i = len; i > pos; --i)
			buf[i + nl - 1] = buf[i - 1];

		for (int i = pos; i < pos + nl; i++)
			buf[i] = tmpbuf[i - pos];
	} else if (ins) {
		for (int i = len; i >= pos; --i)
			buf[i + nl - 1] = buf[i - 1];

		for (int i = pos; i < pos + nl; i++)
			buf[i - 1] = tmpbuf[i - pos];
	} else if (ow) {
		for (int i = len; i != pos; --i)
			buf[i + nl - 2] = buf[i - 1];

		for (int i = pos; i < pos + nl; i++)
			buf[i - 1] = tmpbuf[i - pos];
	}

	free(tmpbuf);
}

void dellines(char **buf, int start, int end)
{
	int len;
	for (len = 0; buf[len]; ++len) ;
	if (end == END || end > len)
		end = len;

	if (end == len) {
		for (int i = start - 1; buf[i]; ++i)
			buf[i] = NULL;
	} else {
		int i, j;
		for (j = end, i = start - 1; i < end; ++i, ++j) {
			if (buf[j])
				buf[i] = buf[j];
			else
				buf[i] = NULL;
		}
		if (buf[j]) {
			for (; buf[j]; j++)
				buf[i++] = buf[j];
		}
		buf[i] = NULL;
	}
}

void movelines(char **buf, int start, int end, int to, int y)
{
	if (to == end) {
		fprintf(stderr, "?\n");
		return;
	}

	char **tmpbuf = (char **)malloc((end - start + 1) * sizeof(char *));
	for (int i = start - 1; i < end; ++i)
		tmpbuf[i - (start - 1)] = strdup(buf[i]);

	if (!y) {
		dellines(buf, start, end);
		to = to > end ? to - (end - start + 1) : to;
	}
	int len;
	for (len = 0; buf[len]; ++len) ;

	for (int i = len; i >= to; --i)
		buf[(end - start + 1) + i - 1] = strdup(buf[i - 1]);
	for (int i = 0; i < (end - start + 1); ++i)
		buf[to + i] = strdup(tmpbuf[i]);

	for (int i = 0; i < (end - start + 1); ++i)
		free(tmpbuf[i]);
	free(tmpbuf);
}

void undo(FILE *tmp, char **buf)
{
	char **tmpbuf = readtmp(tmp);
	int i;
	for (i = 0; tmpbuf[i]; ++i)
		buf[i] = strdup(tmpbuf[i]);
	if (buf[i]) {
		for (; buf[i]; ++i)
			buf[i] = NULL;
	}
	for (i = 0; tmpbuf[i]; ++i)
		free(tmpbuf[i]);
	free(tmpbuf);
}
