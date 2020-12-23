#ifndef HASH
#define HASH
#include "lexer.h"

extern long long int hashIt(char* in);
extern short int* hash();
extern TOKEN getTokenOfSymbol(short int* hashTable, char* in,char checkTable[][30]);


#endif
