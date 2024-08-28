#include <ctype.h>
#include "bettered.h"

struct {
	unsigned int suppress : 1;
	unsigned int warn : 1;
} flags;

extern int len;
int lines[2] = {1, END};

char *cmd = NULL;
char *filename = NULL;
char **buf = NULL;

FILE *tmp;

unsigned long bufhash;

static void freeall();
static int checkname(char *);

// TODO: make parser smaller
int main(int argc, char **argv)
{
	tmp = tmpfile();
	if (argc >= 2) {
		for (int i = 1; i < argc; i++) {
			if (strcmp(argv[i], "-") == 0)
				flags.suppress = 1;
			else if (strcmp(argv[i], "-o") == 0)
				filename = strdup("/dev/stdout");
			else
				filename = strdup(argv[i]);
		}
	}
	if (filename != NULL)
		buf = readfile(!strcmp(filename, "/dev/stdout") ? NULL : filename,
								flags.suppress);
	else
		buf = readfile(NULL, flags.suppress);
	bufhash = hash(buf);
	size_t size = 0;
	while (1) {
		if (getline(&cmd, &size, stdin) <= 0) {
			freeall();
			exit(0);
		}
		struct token *tokens = scanner(cmd);
		for (int i = 0, j = 0; i < len; ++i) {
			switch (tokens[i].type) {
				case BANG:
					system(tokens[i].literal);
					if (!flags.suppress)
						printf("!\n");
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
					writetmp(tmp, buf);
					insertlines(buf, lines[1], 0);
					break;
				case CHANGE:
					writetmp(tmp, buf);
					changelines(buf, lines[0], lines[1]);
					break;
				case INSERT:
					writetmp(tmp, buf);
					insertlines(buf, lines[0], 1);
					break;
				case DELETE:
					writetmp(tmp, buf);
					dellines(buf, lines[0], lines[1]);
					break;
				case JOIN:
					writetmp(tmp, buf);
					joinlines(buf, lines[0], lines[1]);
					break;
				case MOVE:
					writetmp(tmp, buf);
					movelines(buf, lines[0], lines[1], *((int *)tokens[i].literal), 1);
					break;
				case TRANSFER:
					writetmp(tmp, buf);
					movelines(buf, lines[0], lines[1], *((int *)tokens[i].literal), 0);
					break;
				case SUBSTITUTE:
					writetmp(tmp, buf);
					char **pr = (char **) tokens[i].literal;
					substitute(buf, lines[0], lines[1], pr[0], pr[1]);
					for (int k = 0; k < 2; ++k)
						free(pr[k]);
					break;
				case UNDO:
					undo(tmp, &buf);
					break;
				case PRINT_FILENAME:
					if (checkname(tokens[i].literal) == 0)
						fprintf(stderr, "?\n");
					else
						printf("%s\n", filename);
					break;
				case EDIT_CHECK:
					if (hash(buf) != bufhash && !flags.warn) {
						fprintf(stderr, "?\n");
						flags.warn = 1;
					} else {
						if (checkname(tokens[i].literal) == 0)
							fprintf(stderr, "?\n");
						else {
							for (int i = 0; buf[i]; ++i)
								free(buf[i]);
							free(buf);
							buf = readfile(filename, flags.suppress);
							bufhash = hash(buf);
							flags.warn = 0;
						}
					}
					break;
				case EDIT_NOCHECK:
					if (checkname(tokens[i].literal) == 0)
						fprintf(stderr, "?\n");
					else {
						for (int i = 0; buf[i]; ++i)
							free(buf[i]);
						free(buf);
						buf = readfile(filename, flags.suppress);
						bufhash = hash(buf);
					}
					break;
				case READ:
					if (checkname(tokens[i].literal) == 0)
						fprintf(stderr, "?\n");
					else {
						writetmp(tmp, buf);
						appendlines(buf, filename, lines[1], flags.suppress);
					}
					break;
				case APPEND_FILE:
					if(checkname(tokens[i].literal) == 0)
						fprintf(stderr, "?\n");
					else {
						bufhash = hash(buf);
						appendfile(filename, buf, flags.suppress);
					}
					break;
				case WRITE:
					if (checkname(tokens[i].literal) == 0)
						fprintf(stderr, "?\n");
					else {
						bufhash = hash(buf);
						writefile(filename, buf, flags.suppress);
					}
					break;
				case ERROR:
					fprintf(stderr, "?\n");
					break;
				case QUIT_CHECK:
					if (bufhash != hash(buf) && !flags.warn) {
						fprintf(stderr, "?\n");
						flags.warn = 1;
						break;
					}	else {
						freeall();
						free(tokens);
						exit(0);
					}
				case QUIT_NOCHECK:
					freeall();
					free(tokens);
					exit(0);
				case NUMBER:
					if (j < 2)
						lines[j++] = *((int *) tokens[i].literal);
					else
						break;
					if (j == 1)
						lines[j] = lines[j - 1];
					break;
				default:
					printf("%d - %s\n", tokens[i].type, *((char **)tokens[i].literal));
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
	fclose(tmp);
}

static int checkname(char *s) {
	if (strlen(s)) {
		if (filename != NULL)
			free(filename);
		filename = strdup(s);
	}
	return filename == NULL ? 0 : strlen(filename);
}
