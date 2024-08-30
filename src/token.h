#ifndef TOKEN_H
#define TOKEN_H

enum toktype {
  // Single-Character tokens
  BANG = 1, COMMA,

  // Multi-Character tokens
  NUMBER,

  // Commands
  APPEND_FILE, APPEND_LINES, CHANGE, DELETE, EDIT_CHECK, EDIT_NOCHECK,
  GLOBAL, INSERT, INVERT, JOIN, MOVE, PRINT, PRINT_FILENAME, READ,
  SUBSTITUTE, QUIT_CHECK, QUIT_NOCHECK, TRANSFER, UNDO, WRITE,

  ERROR
};

struct token {
  enum toktype type;
  void *literal;
};

extern int len;
struct token *scanner(char *);

#endif
