#include "bufops.h"

static char **getlines(int *);

void printlines(char **buf, int ln, int start, int end)
{
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
	int nl, len;
	char **tmpbuf = getlines(&nl);
	get_len(buf, len);
	if (pos == END || pos > len)
		pos = len;

	if (ins && len > 0) {
		for (int i = len; i >= pos; --i)
			buf[i + nl - 1] = buf[i - 1];
		for (int i = pos; i < pos + nl; i++)
			buf[i - 1] = strdup(tmpbuf[i - pos]);
	} else {
		for (int i = len; i > pos; --i)
			buf[i + nl - 1] = buf[i - 1];
		for (int i = pos; i < pos + nl; i++)
			buf[i] = strdup(tmpbuf[i - pos]);
	}

	for (int i = 0; i < nl; ++i)
		free(tmpbuf[i]);
	free(tmpbuf);
}

void changelines(char **buf, int start, int end)
{
	int nl, len;
	char **tmpbuf = getlines(&nl);
	get_len(buf, len);
	end = end == END || end > len ? len : end;
	start = start == END ? len : start;

	int i, j;
	for (i = start - 1, j = 0; i < end && j < nl; ++i, ++j) {
		free(buf[i]);
		buf[i] = strdup(tmpbuf[j]);
	}
	if (j < nl) {
		for (int tmp = len; tmp > end; --tmp)
			buf[tmp + (nl - j) - 1] = buf[tmp - 1];
		for (; j < nl; ++i, ++j)
			buf[i] = strdup(tmpbuf[j]);
	}
	for (i = 0; i < nl; ++i)
		free(tmpbuf[i]);
	free(tmpbuf);
}

void dellines(char **buf, int start, int end)
{
	int len;
	get_len(buf, len);

	end = end == END || end > len ? len : end;
	start = start == END ? len : start;
	char **tmpbuf = calloc((len - end + 1), sizeof(char *));
	for (int i = end; buf[i]; ++i)
		tmpbuf[i - end] = strdup(buf[i]);

	for (int i = start - 1, j = 0; i < len; ++i, ++j) {
		free(buf[i]);
		if (j < (len - end)) {
			buf[i] = strdup(tmpbuf[j]);
			free(tmpbuf[j]);
		}
		else
			buf[i] = NULL;
	}
	free(tmpbuf);
}

void movelines(char **buf, int start, int end, int to, int cut)
{
	char **tmpbuf = calloc((end - start + 1), sizeof(char *));
	for (int i = start - 1; i < end; ++i)
		tmpbuf[i - (start - 1)] = strdup(buf[i]);

	if (cut) {
		dellines(buf, start, end);
		to = to > end ? to - (end - start + 1) : to;
	}
	int len;
	get_len(buf, len);

	for (int i = len; i >= to; --i)
		buf[(end - start + 1) + i - 1] = buf[i - 1];
	for (int i = 0; i < (end - start + 1); ++i) {
		buf[to + i] = strdup(tmpbuf[i]);
		free(tmpbuf[i]);
	}
	free(tmpbuf);
}

void joinlines(char **buf, int start, int end)
{
	int len;
	get_len(buf, len);
	if (end == END || end > len)
		end = len;
	char **tmpbuf = calloc((end - start), sizeof(char *));
	for (int i = start; i < end; ++i)
		tmpbuf[i - start] = strdup(buf[i]);
	dellines(buf, start + 1, end);
	for (int i = 0; i < (end - start); ++i) {
		char *tmp =
		    malloc(strlen(buf[start - 1]) + strlen(tmpbuf[i]) + 1);
		strcpy(tmp, buf[start - 1]);
		tmp[strlen(buf[start - 1]) - 1] = '\0';
		strcat(tmp, tmpbuf[i]);

		free(buf[start - 1]);
		buf[start - 1] = strdup(tmp);
		free(tmp);
		free(tmpbuf[i]);
	}
	free(tmpbuf);
}

void appendlines(char **buf, char *filename, int pos, int suppress)
{
	int len;
	get_len(buf, len);
	if (pos == END || pos > len)
		pos = len;
	char **tmpbuf = calloc(len - pos, sizeof(char *));
	for (int i = pos; i < len; ++i)
		tmpbuf[i - pos] = strdup(buf[i]);
	char **contents = readfile(filename, suppress);
	int offset;
	get_len(contents, offset);
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
