#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
  // Single-Character tokens
  BANG, SEPERATOR,

  // Multi-Character tokens
  NUMBER,

  // Commands
  APPEND, CHANGE, DELETE, GLOBAL, INSERT,
  JOIN, MOVE, SUBSTITUTE, QUIT, TRANSFER, UNDO,

  ERROR
} token_t;

typedef struct {
  token_t type;
  void *literal;
} Token;

Token *scanner(char *);

#endif
