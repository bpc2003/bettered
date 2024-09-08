#include <stdio.h>
#include <regex.h>
#include <stdlib.h>
#include <string.h>
#include "bufops.h"
#include "regexp.h"

int *find(char **buf, char *pat, int *len, int inv)
{
	int pos = 0;
	int *lines = calloc(2, sizeof(int));
	regex_t re;
	if (lines == NULL || regcomp(&re, pat, 0)) {
		fprintf(stderr, "?\n");
		if (lines)
			free(lines);
		return NULL;
	}

	for (int i = 0; buf[i]; ++i) {
		int comp = regexec(&re, buf[i], 0, NULL, 0);
		if (comp == 0 && !inv || comp == REG_NOMATCH && inv) {
			if (pos >= 2)
				lines = realloc(lines, (pos + 1) * sizeof(int));
			lines[pos++] = i + 1;
		}
	}
	*len = pos;
	regfree(&re);
	return lines;
}

// TODO: make function smaller
void substitute(char **buf, int start, int end, char *pat, char *rep)
{
	regex_t re;
	regmatch_t pmatch[1];
	if (regcomp(&re, pat, 0)) {
		fprintf(stderr, "?\n");
		return;
	}
	if (end == END)
		end = get_len(buf);
	for (int i = start - 1; i < end; ++i) {
		if (regexec(&re, buf[i], 1, pmatch, 0) != REG_NOMATCH) {
			if (strlen(rep) <= strlen(pat)) {
				for (int j = pmatch[0].rm_so, p = 0; j < strlen(buf[i]); ++j) {
					if (p < strlen(rep))
						buf[i][j] = rep[p++];
					else if (strlen(pat) > 1)
						buf[i][j] = buf[i][pmatch[0].rm_eo + j - p];
				}
			} else {
				buf[i] = realloc(buf[i], strlen(buf[i]) +
						 strlen(rep) - strlen(pat) + 1);
				for (int j = strlen(buf[i]); j >= pmatch[0].rm_eo - 1; --j)
					buf[i][j + (strlen(rep) - strlen(pat))] = buf[i][j];
				for (int j = pmatch[0].rm_so; j < strlen(rep) + pmatch[0].rm_so; ++j)
					buf[i][j] = rep[j - pmatch[0].rm_so];
			}
			printf("%s", buf[i]);
		}
	}
	regfree(&re);
}
