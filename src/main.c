#include <ctype.h>
#include "bed.h"
#include "token.h"

struct {
	unsigned int suppress : 1;
} flags;

extern int len;
int lines[3] = {1, END, END};

// TODO: make parser smaller
int main(int argc, char **argv)
{
	char *cmd = NULL;
	size_t size = 0;
	while (1) {
		getline(&cmd, &size, stdin);
		struct token *tokens = scanner(cmd);
		for (int i = 0, j = 0; i < len; ++i) {
			switch (tokens[i].type) {
				case BANG:
					system(tokens[i].literal);
					free(tokens[i].literal);
					break;
				case ERROR:
					fprintf(stderr, "?\n");
					break;
				case QUIT:
					free(cmd);
					free(tokens[i].literal);
					free(tokens);
					exit(0);
				case NUMBER:
					lines[j++] = atoi(tokens[i].literal);
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
