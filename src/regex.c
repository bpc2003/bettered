#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bled.h"

int *find(char **buf, char *pat, int *len)
{
	int val, pos;
	int *lines = (int *)calloc(1, sizeof(int));
	if (lines == NULL) {
		perror("Err");
		return NULL;
	}

	regex_t re;

	val = regcomp(&re, pat, 0);
	if (val) {
		fprintf(stderr, "?\n");
		return NULL;
	}
	pos = 0;
	for (int i = 0; buf[i]; ++i) {
		val = regexec(&re, buf[i], 0, NULL, 0);
		if (val == REG_NOMATCH)
			continue;
		else if (val == 0) {
			lines = (int *)realloc(lines, (++pos) * sizeof(int));
			lines[pos - 1] = i + 1;
		}
	}

	*len = pos;
	regfree(&re);
	return lines;
}

void substitute(char **buf, int start, int end, char *pat, char *rep)
{
	int val;
	regex_t re;
	regmatch_t pmatch[1];

	val = regcomp(&re, pat, 0);
	if (val) {
		fprintf(stderr, "?\n");
		return;
	}

	for (int i = 0; buf[i]; ++i) {
		val = regexec(&re, buf[i], 1, pmatch, 0);
		if (val == 0) {
			int p = 0;
			int j;
			for (j = pmatch[0].rm_so; j < pmatch[0].rm_eo; ++j) {
				if (rep[p])
					buf[i][j] = rep[p++];
				else {
					for (; buf[i][j]; ++j)
						buf[i][j] = buf[i][j + 1];
				}
			}
			if (rep[p]) {
				buf[i] =
				    (char *)realloc(buf[i],
						    strlen(buf[i]) +
						    (strlen(rep) - (p + 1)));
				for (int k = strlen(buf[i]); k >= p + 1; --k)
					buf[i][k + (strlen(rep) - (p + 1))] =
					    buf[i][k - 1];
				for (; rep[p]; p++)
					buf[i][j++] = rep[p];
			}
		}
	}
	regfree(&re);
}
