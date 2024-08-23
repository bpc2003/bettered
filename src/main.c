#include <ctype.h>
#include "bed.h"
#include "token.h"

struct {
	unsigned int suppress : 1;
} flags;

// TODO: make parser smaller
int main(int argc, char **argv)
{
	char *cmd = NULL;
	size_t size = 0;
	while (1) {
		getline(&cmd, &size, stdin);
		Token *tokens = scanner(cmd);
		for (int i = 0; tokens[i].type; ++i) {
			switch (tokens[i].type) {
				case QUIT:
					free(cmd);
					free(tokens[i].literal);
					free(tokens);
					exit(0);
				default:
					printf("%d - %s\n", tokens[i].type, tokens[i].literal);
					break;
			}
			if (tokens[i].literal != NULL)
				free(tokens[i].literal);
		}
		free(tokens);
	}
}
