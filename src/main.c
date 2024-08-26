#include <ctype.h>
#include "bed.h"
#include "token.h"

struct {
	unsigned int suppress : 1;
} flags;

extern int len;
int lines[2] = {1, END};

char *cmd = NULL;
char *filename = NULL;
char **buf = NULL;

static void freeall();

// TODO: make parser smaller
int main(int argc, char **argv)
{
	if (argc == 2)
		filename = strdup(argv[1]);
	else if (argc > 2)
		exit(1);
	buf = readfile(filename, 0);
	size_t size = 0;
	while (getline(&cmd, &size, stdin) > 0) {
		struct token *tokens = scanner(cmd);
		for (int i = 0, j = 0; i < len; ++i) {
			switch (tokens[i].type) {
				case BANG:
					system(tokens[i].literal);
					break;
				case COMMA:
					lines[0] = 1;
					lines[1] = END;
					break;
				case PRINT:
					printlines(buf, !strcmp(tokens[i].literal, "n"),
								lines[0], lines[1]);
					break;
				case APPEND_LINES:
					insertlines(buf, lines[1], 0);
					break;
				case CHANGE:
					changelines(buf, lines[0], lines[1]);
					break;
				case INSERT:
					insertlines(buf, lines[0], 1);
					break;
				case DELETE:
					dellines(buf, lines[0], lines[1]);
					break;
				case JOIN:
					joinlines(buf, lines[0], lines[1]);
					break;
				case MOVE:
					if (strlen(tokens[i].literal) == 0)
						fprintf(stderr, "?\n");
					else
						movelines(buf, lines[0], lines[1], atoi(tokens[i].literal), 1);
					break;
				case TRANSFER:
					if (strlen(tokens[i].literal) == 0)
						fprintf(stderr, "?\n");
					else
						movelines(buf, lines[0], lines[1], atoi(tokens[i].literal), 0);
					break;
				case EDIT_CHECK:
				case EDIT_NOCHECK:
					if (filename != NULL)
						free(filename);
					filename = strdup(tokens[i].literal);
					for (int i = 0; buf[i]; ++i)
						free(buf[i]);
					free(buf);
					buf = readfile(filename, 0);
					break;
				case WRITE:
					writefile(filename, buf, 0);
					break;
				case ERROR:
					fprintf(stderr, "?\n");
					break;
				case QUIT:
					freeall();
					free(tokens[i].literal);
					free(tokens);
					exit(0);
				case NUMBER:
					if (j < 2)
						lines[j++] = atoi(tokens[i].literal);
					else
						break;
					if (j == 1)
						lines[j] = lines[j - 1];
					break;
				default:
					printf("%d - %s\n", tokens[i].type, tokens[i].literal);
					break;
			}
			if (tokens[i].literal != NULL)
				free(tokens[i].literal);
		}
		len = 0;
		free(tokens);
	}
}

static void freeall() {
	free(cmd);
	free(filename);
	for (int i = 0; buf[i]; ++i)
		free(buf[i]);
	free(buf);
}
