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

	int nl = 0;

	while (1) {
		getline(&tmp, &size, stdin);
		if (!strcmp(tmp, ".\n"))
			break;
		tmpbuf =
		    (char **)realloc(tmpbuf, sizeof(char *) * sizeof(tmpbuf));
		tmpbuf[nl++] = strdup(tmp);
	}

	int len = 0;
	for (; buf[len]; len++) ;

	if (!ins && !ow) {
		for (int i = len; i > pos; --i)
			buf[i + nl - 1] = strdup(buf[i - 1]);

		for (int i = pos; i < pos + nl; i++)
			buf[i] = strdup(tmpbuf[i - pos]);
	} else if (ins) {
		for (int i = len; i >= pos; --i)
			buf[i + nl - 1] = strdup(buf[i - 1]);

		for (int i = pos; i < pos + nl; i++)
			buf[i - 1] = strdup(tmpbuf[i - pos]);
	} else if (ow) {
		for (int i = len; i != pos; --i)
			buf[i + nl - 2] = strdup(buf[i - 1]);

		for (int i = pos; i < pos + nl; i++)
			buf[i - 1] = strdup(tmpbuf[i - pos]);
	}

	free(tmpbuf);
}

void dellines(char **buf, int start, int end)
{
	int len;
	for (len = 0; buf[len]; ++len) ;
	if (end > len) {
		fprintf(stderr, "?\n");
		return;
	}

	if (end == END || end == len) {
		for (int i = start - 1; buf[i]; ++i)
			buf[i] = NULL;
	} else if (start != end) {
		int i, j;
		for (j = end, i = start - 1; i < end; ++i, ++j) {
			if (buf[j])
				buf[i] = strdup(buf[j]);
			else
				buf[i] = NULL;
		}
		if(buf[j]) {
			for(; buf[j]; j++)
				buf[i++] = buf[j];
			buf[i] = NULL;
		}
	} else {
		int i;
		for (i = start - 1; i < len; ++i)
			buf[i] = buf[i + 1];
		buf[i] = NULL;
	}
}
