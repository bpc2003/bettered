#include "bed.h"
#include "token.h"

static void addtok(Token *, int *, token_t, void *);

Token *scanner(char *src)
{
  Token *tokens = calloc(1, sizeof(Token));
  int pos = 0;
  if (tokens == NULL)
    return NULL;
  
  for (int i = 0; i < strlen(src); ++i) {
    switch (src[i]) {
      case 'q':
      case 'Q':
        addtok(tokens, &pos, QUIT, NULL);
        break;
      default:
        addtok(tokens, &pos, ERROR, "?");
        break;
    }
  }
  return tokens;
}

static void addtok(Token *tokens, int *pos, token_t type, void *literal)
{
  tokens = realloc(tokens, 1);
  tokens[*pos++] = (Token) { .type = type, .literal = literal };
}
