#include <stdlib.h>
#include <string.h>
#include "bufops.h"
#include "fileops.h"

static char **getlines(int *);

void printlines(char **buf, int ln, int start, int end)
{
	if (start == END)
		start = get_len(buf);
	for (int i = start - 1; buf[i]; ++i) {
		if (i == end)
			break;
		if (ln)
			printf("%d\t%s", i + 1, buf[i]);
		else
			printf("%s", buf[i]);
	}
}

void insertlines(char **buf, int pos, int ins)
{
	int nl, len = get_len(buf);
	char **tmpbuf = getlines(&nl);
	if (pos == END || pos > len)
		pos = len;

	tmpbuf = realloc(tmpbuf, (nl + len) * sizeof(char *));
	if (ins && len > 0)
		--pos;
	memmove(tmpbuf + nl, buf + pos, len * sizeof(char *));
	memcpy(buf + pos, tmpbuf, (len + nl) * sizeof(char *));
	free(tmpbuf);
}

void changelines(char **buf, int start, int end)
{
	int nl, len = get_len(buf);
	char **tmpbuf = getlines(&nl);
	end = end == END || end > len ? len : end;
	start = start == END || start > len ? len : start;

	for (int i = start - 1; i < end; ++i)
		free(buf[i]);
	tmpbuf = realloc(tmpbuf, (nl + len) * sizeof(char *));
	memmove(tmpbuf + nl, buf + end, len * sizeof(char *));
	memcpy(buf + start - 1, tmpbuf, (len + nl) * sizeof(char *));
	free(tmpbuf);
}

void dellines(char **buf, int start, int end)
{
	int len = get_len(buf);
	end = end == END || end > len ? len : end;
	start = start == END || start > len ? len : start;

	char **tmpbuf = calloc((len - end + 1), sizeof(char *));
	memmove(tmpbuf, buf + end, (len - end + 1) * sizeof(char *));

	for (int i = start - 1; i < end; ++i) {
		free(buf[i]);
		buf[i] = NULL;
	}
	memcpy(buf + start - 1, tmpbuf, (len - end + 1) * sizeof(char *));
	free(tmpbuf);
}

void movelines(char **buf, int start, int end, int to, int cut)
{
	if (start == 1 && end == END)
		return;
	char **tmpbuf = calloc((end - start + 1), sizeof(char *));
	for (int i = start - 1; i < end; ++i)
		tmpbuf[i - (start - 1)] = strdup(buf[i]);

	if (cut) {
		dellines(buf, start, end);
		to = to > end ? to - (end - start + 1) : to;
	}
	int len = get_len(buf);

	for (int i = len; i > to; --i)
		buf[(end - start + 1) + i - 1] = buf[i - 1];
	for (int i = 0; i < (end - start + 1); ++i) {
		buf[to + i] = strdup(tmpbuf[i]);
		free(tmpbuf[i]);
	}
	free(tmpbuf);
}

void joinlines(char **buf, int start, int end)
{
	int len = get_len(buf);
	end = end == END || end > len ? len : end;
	if (start >= len) {
		fprintf(stderr, "?\n");
		return;
	}

	len = strlen(buf[start - 1]);
	char *tmp = calloc(len, sizeof(char));
	strncpy(tmp, buf[start - 1], len - 1);
	for (int i = start; i < end; ++i) {
		len += strlen(buf[i]);
		tmp = realloc(tmp, len);
		tmp = strncat(tmp, buf[i], strlen(buf[i]) - 1);
	}
	tmp = strcat(tmp, "\n");
	buf[start - 1] = realloc(buf[start - 1], len + 1);
	strcpy(buf[start - 1], tmp);
	free(tmp);
	dellines(buf, start + 1, end);
}

void appendlines(char **buf, char *filename, int pos, int suppress)
{
	int len = get_len(buf);
	if (pos == END || pos > len)
		pos = len;
	char **tmpbuf = calloc(len - pos, sizeof(char *));
	for (int i = pos; i < len; ++i)
		tmpbuf[i - pos] = strdup(buf[i]);
	char **contents = readfile(filename, suppress);
	int offset = get_len(contents);
	for (int i = 0; i < len - pos; ++i) {
		buf[i + pos + offset] = strdup(tmpbuf[i]);
		free(tmpbuf[i]);
	}
	free(tmpbuf);
	for (int i = 0; contents[i]; i++) {
		free(buf[i + pos]);
		buf[i + pos] = strdup(contents[i]);
		free(contents[i]);
	}
	free(contents);
}

void undo(FILE *tmp, char ***buf)
{
	for (int i = 0; (*buf)[i]; ++i)
		free((*buf)[i]);
	free(*buf);
	*buf = readtmp(tmp);
}

static char **getlines(int *nl)
{
	char **tmpbuf = calloc(2, sizeof(char *));
	char *tmp = NULL;
	size_t size = 0;
	int i = 0;
	while (1) {
		getline(&tmp, &size, stdin);
		if (!strcmp(tmp, ".\n"))
			break;
		if (i > 1)
			tmpbuf = realloc(tmpbuf, sizeof(char *) * (i + 1));
		tmpbuf[i++] = strdup(tmp);
	}
	if (i >= BUFSIZ) {
		fprintf(stderr, "?\n");
		return NULL;
	}
	free(tmp);
	*nl = i;
	return tmpbuf;
}
