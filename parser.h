#ifndef PARSER
#define PARSER
#define NumNonTerminals 26

#include"node.h"

typedef enum{
    Program,
    Functions,
    MainFun,
    OtherFuns,
    Funct,
    Parameters,
    InList,
    IDList,
    RemId,
    OutId,
    FunBody,
    Declarations,
    Declaration,
    OtherStatements,
    OtherStatementsDash,
    Statement,
    AssignmentStmt,
    AssignmentStmtDash,
    Expression,
    FunctionCall,
    ConditionalStmt,
    BoolExp,
    BoolExpDash,
    RepetitiveStmt,
    ReturnStmt,
    IO_Stmt
}nonTerminals;

extern ptrGnode* buildGrammar();
extern void genParseTable(ptrGnode* gArray);



#endif
