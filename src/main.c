#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "bed.h"

FILE *tmp;
char *shcmd;
char *filename;
char **buf;
char *pat;
char *rep;
int flag = 0;
unsigned long bufhash;

struct {
	unsigned int suppress : 1;
} flags;

void command(char, int, int, int);

int main(int argc, char **argv)
{
	flags.suppress = 0;
	if (argc >= 2) {
		*++argv;
		while(*argv) {
			if (!strcmp(*argv, "-"))
				flags.suppress = 1;
			else if(*argv[0] == '-') {
				switch (*argv[1]) {
					default:
						fprintf(stderr, "usage: - [suppress output from w and e commands]\n");
						exit(1);
				}
			}
			else
				filename = strdup(*argv);
			*++argv;
		}
	}
	tmp = tmpfile();

	shcmd = calloc(100, sizeof(char));
	char *cmdstr = NULL;
	size_t size = 0;

	pat = calloc(100, sizeof(char));
	rep = calloc(100, sizeof(char));
	int *lines, lineslen = 0;

	buf = readfile(filename, flags.suppress);
	if (buf == NULL)
		return 1;
	bufhash = hash(buf);

	int start = 1;
	int end = END;
	while (1) {
		getline(&cmdstr, &size, stdin);
		char cmd;

		int dst = 0;
		int i = 0;

		while (!isalpha(cmdstr[i]) && cmdstr[i] != '!') {
			if (isdigit(cmdstr[i])) {
				for (end = 0; isdigit(cmdstr[i]); i++)
					end = 10 * end + (cmdstr[i] - '0');
				start = end;
				if (cmdstr[i] == ',')
					i++;
				else
					break;
				if (cmdstr[i] == '$') {
					end = END;
					i++;
				} else {
					for (end = 0; isdigit(cmdstr[i]); i++)
						end =
						    10 * end + (cmdstr[i] -
								'0');
				}
			} else if (cmdstr[i] == ',') {
				start = 1;
				end = END;
				i++;
			} else {
				break;
			}
		}

		cmd = cmdstr[i];
		if (!isalpha(cmd) && cmd != '!')
			continue;
		char *str;
		if (cmdstr[0] == '!')
			str = shcmd;
		else if (cmdstr[1] == '/' || cmdstr[1] == '?'
			 || cmdstr[1] == '\\')
			str = pat;
		if (cmdstr[i + 1]) {
			while (cmdstr[i]) {
				if (isdigit(cmdstr[i]))
					dst = 10 * dst + (cmdstr[i] - '0');
				else if (cmdstr[0] == '!') {
					if (i < 98 && cmdstr[i])
						*str++ = cmdstr[i + 1];
				} else if ((cmdstr[i] == '/' ||
						cmdstr[i] == '?' || cmdstr[i] == '\\')
						&& (cmd == 'g' || cmd == 's')) {
					char t = cmdstr[i];
					if (i > 1) {
						t = '\n';
						str = rep;
					}
					while (cmdstr[++i] != t)
						*str++ = cmdstr[i];
					*str = '\0';
					continue;
				} else if (cmdstr[i] == ' ') {
					cmdstr[strlen(cmdstr) - 1] = '\0';
					free(filename);
					filename = strdup(cmdstr + i + 1);
				}
				++i;
			}
		}

		if (cmd == 'g') {
			lines = find(buf, pat, &lineslen);
			cmd = cmdstr[i - 2];
			if (lineslen == 0)
				continue;
		}
		if (lineslen > 0) {
			int *it = lines;
			while (lineslen > 0) {
				command(cmd, *it, *it, dst);
				*it++;
				lineslen--;
			}
			free(lines);
			continue;
		}

		command(cmd, start, end, dst);
	}
}

void command(char cmd, int start, int end, int dst)
{
	switch (cmd) {
	case 'p':
	case 'n':
		printlines(buf, cmd == 'n', start, end);
		break;
	case 'a':
		writetmp(tmp, buf);
		insertlines(buf, end, 0);
		break;
	case 'i':
		writetmp(tmp, buf);
		insertlines(buf, start, 1);
		break;
	case 'c':
		writetmp(tmp, buf);
		changelines(buf, start, end);
		break;
	case 'd':
		writetmp(tmp, buf);
		dellines(buf, start, end);
		break;
	case 'm':
	case 't':
		writetmp(tmp, buf);
		movelines(buf, start, end, dst, cmd == 'm');
		break;
	case 'j':
		writetmp(tmp, buf);
		joinlines(buf, start, end);
		break;
	case 's':
		writetmp(tmp, buf);
		substitute(buf, start, end, pat, rep);
		break;
	case 'e':
		if(bufhash == hash(buf) || flag) {
			flag = 0;
			for (int i = 0; buf[i]; ++i)
					free(buf[i]);
			free(buf);
			buf = readfile(filename, flags.suppress);
			bufhash = hash(buf);
		} else {
			flag = 1;
			fprintf(stderr, "?\n");
		}
		break;
	case 'E':
		for (int i = 0; buf[i]; ++i)
			free(buf[i]);
		free(buf);
		buf = readfile(filename, flags.suppress);
		bufhash = hash(buf);
		break;
	case 'W':
		bufhash = hash(buf);
		appendfile(filename, buf, flags.suppress);
		break;
	case 'w':
		bufhash = hash(buf);
		writefile(filename, buf, flags.suppress);
		break;
	case 'u':
		undo(tmp, &buf);
		break;
	case '!':
		system(shcmd);
		break;
	case 'q':
		if (hash(buf) != bufhash && !flag) {
			fprintf(stderr, "?\n");
			flag = 1;
			break;
		}
		else {
			for (int i = 0; buf[i]; ++i)
				free(buf[i]);
			free(buf);
			free(shcmd);
			free(pat);
			free(rep);
			free(filename);
			fclose(tmp);
			exit(0);
		}
	case 'Q':
		for (int i = 0; buf[i]; ++i)
			free(buf[i]);
		free(buf);
		free(shcmd);
		free(pat);
		free(rep);
		free(filename);
		fclose(tmp);
		exit(0);
	default:
		fprintf(stderr, "?\n");
		break;
	}
}
