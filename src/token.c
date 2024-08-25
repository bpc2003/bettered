#include "bed.h"
#include "token.h"

static void addtok(struct token **, int *, enum toktype, void *);
static char *getpat(char *, int *);
static char *getint(char *, int *);

int len = 0;

struct token *scanner(char *src)
{
  struct token *tokens = calloc(2, sizeof(struct token));
  int pos = 0;
  if (tokens == NULL)
    return NULL;

  for (int i = 0; i < strlen(src); ++i) {
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
      case 'g':
        addtok(&tokens, &pos, GLOBAL, getpat(src, &i));
        break;
      case 's':
        addtok(&tokens, &pos, SUBSTITUTE, getpat(src, &i));
        break;
      case 'u':
        addtok(&tokens, &pos, UNDO, NULL);
        break;
      case 'r':
        addtok(&tokens, &pos, READ, strndup(src + 2, strlen(src + 2) - 1));
        return tokens;
      case 'e':
        addtok(&tokens, &pos, EDIT_CHECK, strndup(src + 2,
                                                  strlen(src + 2) - 1));
        return tokens;
      case 'E':
        addtok(&tokens, &pos, EDIT_NOCHECK, strndup(src + 2,
                                                    strlen(src + 2) - 1));
        return tokens;
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
        if (src[i] >= '0' && src[i] <= '9')
          addtok(&tokens, &pos, NUMBER, getint(src, &i));
        else
          addtok(&tokens, &pos, ERROR, NULL);
        break;
    }
  }
  return tokens;
}

static void addtok(struct token **tokens, int *pos, enum toktype type, void *literal)
{
  if (*pos > 1)
    *tokens = realloc(*tokens, sizeof(struct token) * (*pos + 1));
  (*tokens)[(*pos)++] = (struct token) { .type = type, .literal = literal };
  ++len;
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

static char *getint(char *src, int *srcpos)
{
  int start = *srcpos;
  while (src[start] >= '0' && src[start] <= '9')
    start++;
  char *num = strndup(src + *srcpos, start - *srcpos);
  if (src[start] == ',')
    *srcpos = start;
  return num;
}
