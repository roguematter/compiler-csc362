#ifndef LEXER
#define LEXER
#define NumTokens 36
#define NumGRules 53
#include <stdio.h>
typedef enum token_set{
    TK_KEY_PRINT,
    TK_KEY_GET,
    TK_KEY_RETURN,
    TK_KEY_BEGIN,
    TK_KEY_END,
    TK_KEY_WHILE,
    TK_KEY_IF,
    TK_KEY_FUN,
    TK_KEY_VAR,
    TK_KEY_NONE,
    TK_KEY_IN,
    TK_KEY_OUT,
    TK_KEY_MAIN,
    TK_KEY_LET,
    TK_KEY_BE,
    TK_KEY_PLUS,
    TK_KEY_MINUS,
    TK_KEY_MUL,
    TK_KEY_DIV,
    TK_KEY_MODULO,
    TK_ID,
    TK_NUM,
    TK_OPEN,
    TK_CLOSE,
    TK_SEMI,
    TK_COLON,
    TK_EQUAL,
    TK_NOTEQUAL,
    TK_LT,
    TK_GT,
    TK_LEQ,
    TK_GEQ,
    TK_COMMA,
    TK_ERROR,
    TK_EOF,
    TK_EPS
} TOKEN;

typedef struct{
	TOKEN tk;
	int line;
	char* pString;
	} TOKENTABLE;

extern TOKEN nextToken();
extern void printTokenStream(FILE* f);
extern void initializeScanner(char *filename);
#endif
