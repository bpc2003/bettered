#include "bettered.h"
#include "token.h"

static void addtok(struct token **, int *, enum toktype, void *);
static int getint(char *, int *);
static char *getpat(char *, int *);
static char **getpr(char *, int *);

int len = 0;
char *pat;

struct token *scanner(char *src)
{
  struct token *tokens = calloc(2, sizeof(struct token));
  int p, pos = 0;
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
      case 'P':
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
        i++;
        if ((p = getint(src, &i)) >= 0) {
          void *n = calloc(1, sizeof(int));
          *((int *) n) = p;
          addtok(&tokens, &pos, MOVE, n);
          break;
        } else {
          addtok(&tokens, &pos, ERROR, NULL);
          return tokens;
        }
      case 't':
        i++;
        if((p = getint(src, &i)) >= 0) {
          void *n = calloc(1, sizeof(int));
          *((int *) n) = p;
          addtok(&tokens, &pos, TRANSFER, n);
          break;
        } else {
          addtok(&tokens, &pos, ERROR, NULL);
          return tokens;
        }
      case 'g':
        pat = getpat(src, &i);
        if (pat == NULL) {
          addtok(&tokens, &pos, ERROR, NULL);
          return tokens;
        } else {
          addtok(&tokens, &pos, GLOBAL, pat);
          break;
        }
      case 'v':
        pat = getpat(src, &i);
        if (pat == NULL) {
          addtok(&tokens, &pos, ERROR, NULL);
          return tokens;
        } else {
          addtok(&tokens, &pos, INVERT, pat);
          break;
        }
      case 's':
        char **pr = getpr(src, &i);
        if (pr[0] == NULL || strlen(pr[1]) == 0)
          addtok(&tokens, &pos, ERROR, NULL);
        else
          addtok(&tokens, &pos, SUBSTITUTE, pr);
        return tokens;
      case 'u':
        addtok(&tokens, &pos, UNDO, NULL);
        break;
      case 'f':
        addtok(&tokens, &pos, PRINT_FILENAME, strndup(src + 2,
                                                      strlen(src + 2) - 1));
        return tokens;
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
        addtok(&tokens, &pos, APPEND_FILE, strndup(src + 2,
                                                   strlen(src + 2) - 1));
        return tokens;
      case 'w':
        addtok(&tokens, &pos, WRITE, strndup(src + 2, strlen(src + 2) - 1));
        return tokens;
      case 'q':
        addtok(&tokens, &pos, QUIT_CHECK, NULL);
        break;
      case 'Q':
        addtok(&tokens, &pos, QUIT_NOCHECK, NULL);
        break;
      case '\n':
        break;
      default:
        if (src[i] >= '0' && src[i] <= '9') {
          void *n = calloc(1, sizeof(int));
          *((int *) n) = getint(src, &i);
          addtok(&tokens, &pos, NUMBER, n);
        }
        else if (src[i] == '$') {
            void *n = calloc(1, sizeof(int));
            *((int *) n) = -1;
            addtok(&tokens, &pos, NUMBER, n);
        }
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
  for (; src[*srcpos] != '/' && src[*srcpos]; (*srcpos)++) ;

  if (src[*srcpos] == '\0')
    return NULL;
  char *pat = strndup(src + start, *srcpos - start);
  return pat;
}

static char **getpr(char *src, int *srcpos)
{
  char **pr = calloc(2, sizeof(char *));
  pr[0] = getpat(src, srcpos);
  ++*srcpos;
  pr[1] = strndup(src + *srcpos, strlen(src + *srcpos) - 1);

  return pr;
}

static int getint(char *src, int *srcpos)
{
  int start = *srcpos;
  while (src[start] >= '0' && src[start] <= '9')
    start++;

  char *tmp = strndup(src + *srcpos, start - *srcpos);
  if (strlen(tmp) == 0) {
    free(tmp);
    return -1;
  }
  int n = atoi(tmp);
  free(tmp);
  if (src[start] == ',')
    *srcpos = start;
  return n;
}
