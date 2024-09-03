#ifndef TOKEN_H
#define TOKEN_H

#include <ctype.h>

enum toktype {
  // Single-Character tokens
  BANG = 1, COMMA, SINGLE_QUOTE,

  // Multi-Character tokens
  NUMBER,

  // Commands
  APPEND_FILE, APPEND_LINES, CHANGE, DELETE, EDIT_CHECK, EDIT_NOCHECK,
  GLOBAL, INSERT, INVERT, JOIN, MARK, MOVE, PRINT, PRINT_FILENAME,
  READ, SUBSTITUTE, QUIT_CHECK, QUIT_NOCHECK, TRANSFER, UNDO, WRITE,

  ERROR
};

struct token {
  enum toktype type;
  void *literal;
};

extern int len;
struct token *scanner(char *);

#endif
