#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
  // Single-Character tokens
  BANG = 1, COMMA,

  // Multi-Character tokens
  NUMBER, STRING,

  // Commands
  APPEND_FILE, APPEND_LINES, CHANGE, DELETE, GLOBAL, INSERT,
  JOIN, MOVE, PRINT, READ, SUBSTITUTE, QUIT, TRANSFER, UNDO,
  WRITE,

  ERROR
} token_t;

typedef struct {
  token_t type;
  void *literal;
} Token;

Token *scanner(char *);

#endif
