#include "bed.h"
#include "token.h"

static void addtok(Token **, int *, token_t, void *);
static char *getpat(char *, int *);

Token *scanner(char *src)
{
  Token *tokens = calloc(2, sizeof(Token));
  int pos = 0;
  if (tokens == NULL)
    return NULL;
 
  for (int i = 0; i < strlen(src) - 1; ++i) {
    switch (src[i]) {
      case '!':
        addtok(&tokens, &pos, BANG, strndup(src + 1, strlen(src + 1) - 1));
        return tokens;
      case ',':
        addtok(&tokens, &pos, COMMA, NULL);
        break;
      case 'p':
      case 'n':
        addtok(&tokens, &pos, PRINT, strndup(src + i, 1));
        break;
      case 'a':
        addtok(&tokens, &pos, APPEND_LINES, NULL);
        break;
      case 'c':
        addtok(&tokens, &pos, CHANGE, NULL);
        break;
      case 'i':
        addtok(&tokens, &pos, INSERT, NULL);
        break;
      case 'd':
        addtok(&tokens, &pos, DELETE, NULL);
        break;
      case 'j':
        addtok(&tokens, &pos, JOIN, NULL);
        break;
      case 'm':
        addtok(&tokens, &pos, MOVE, NULL);
        break;
      case 't':
        addtok(&tokens, &pos, TRANSFER, NULL);
        break;
      case 'g': // Get pattern here
        addtok(&tokens, &pos, GLOBAL, getpat(src, &i));
        break;
      case 's': // Get pattern here
        addtok(&tokens, &pos, SUBSTITUTE, getpat(src, &i));
        break;
      case 'u':
        addtok(&tokens, &pos, UNDO, NULL);
        break;
      case 'r':
        addtok(&tokens, &pos, READ, NULL);
        break;
      case 'W':
        addtok(&tokens, &pos, APPEND_FILE, NULL);
        break;
      case 'w':
        addtok(&tokens, &pos, WRITE, NULL);
        break;
      case 'q':
      case 'Q':
        addtok(&tokens, &pos, QUIT, strndup(src + i, 1));
        break;
      case '\n':
        break;
      default:
        addtok(&tokens, &pos, ERROR, strdup("?"));
        break;
    }
  }
  return tokens;
}

static void addtok(Token **tokens, int *pos, token_t type, void *literal)
{
  if (*pos > 1)
    *tokens = realloc(*tokens, sizeof(Token) * (*pos + 1));
  (*tokens)[(*pos)++] = (Token) { .type = type, .literal = literal };
}

static char *getpat(char *src, int *srcpos)
{
  if (src[++*srcpos] != '/')
    return NULL;
  int start = ++*srcpos;
  for (; src[*srcpos] != '/'; (*srcpos)++) ;

  char *pat = strndup(src + start, *srcpos - start);
  return pat;
}
