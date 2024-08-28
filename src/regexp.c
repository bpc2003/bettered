#include "regexp.h"

int *find(char **buf, char *pat, int *len)
{
	int pos = 0;
	*len = pos;
	int *lines = calloc(1, sizeof(int));
	regex_t re;
	if (lines == NULL)
		return NULL;
	if (regcomp(&re, pat, 0)) {
		fprintf(stderr, "?\n");
		return NULL;
	}

	for (int i = 0; buf[i]; ++i) {
		if (regexec(&re, buf[i], 0, NULL, 0) == 0) {
			lines = realloc(lines, (++pos) * sizeof(int));
			lines[pos - 1] = i + 1;
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
		get_len(buf, end);
	for (int i = start - 1; i < end; ++i) {
		if (regexec(&re, buf[i], 1, pmatch, 0) != REG_NOMATCH) {
			if (strlen(rep) <= strlen(pat)) {
				for (int j = pmatch[0].rm_so, p = 0; j < strlen(buf[i]); ++j) {
					if (p < strlen(rep))
						buf[i][j] = rep[p++];
					else
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
