#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"parser.h"
#include"lexer.h"

int getToken(char *in){
    if(strcmp(in,"TK_KEY_PRINT") == 0)
        return TK_KEY_PRINT;
    else if(strcmp(in,"TK_KEY_GET") == 0)
        return TK_KEY_GET;
    else if(strcmp(in,"TK_KEY_RETURN") == 0)
        return TK_KEY_RETURN;
    else if(strcmp(in,"TK_KEY_BEGIN") == 0)
        return TK_KEY_BEGIN;
    else if(strcmp(in,"TK_KEY_END") == 0)
    	return TK_KEY_END;
    else if(strcmp(in,"TK_KEY_WHILE") == 0)
    	return TK_KEY_WHILE;
    else if(strcmp(in,"TK_KEY_IF") == 0)
    	return TK_KEY_IF;
    else if(strcmp(in,"TK_KEY_FUN") == 0)
    	return TK_KEY_FUN;
    else if(strcmp(in,"TK_KEY_VAR") == 0)
    	return TK_KEY_VAR;
    else if(strcmp(in,"TK_KEY_NONE") == 0)
    	return TK_KEY_NONE;
    else if(strcmp(in,"TK_KEY_IN") == 0)
    	return TK_KEY_IN;
    else if(strcmp(in,"TK_KEY_OUT") == 0)
    	return TK_KEY_OUT;
    else if(strcmp(in,"TK_KEY_MAIN") == 0)
    	return TK_KEY_MAIN;
    else if(strcmp(in,"TK_KEY_LET") == 0)
    	return TK_KEY_LET;
    else if(strcmp(in,"TK_KEY_BE") == 0)
    	return TK_KEY_BE;
    else if(strcmp(in,"TK_KEY_PLUS") == 0)
    	return TK_KEY_PLUS;
    else if(strcmp(in,"TK_KEY_MINUS") == 0)
    	return TK_KEY_MINUS;
    else if(strcmp(in,"TK_KEY_MUL") == 0)
    	return TK_KEY_MUL;
    else if(strcmp(in,"TK_KEY_DIV") == 0)
    	return TK_KEY_DIV;
    else if(strcmp(in,"TK_KEY_MODULO") == 0)
    	return TK_KEY_MODULO;
    else if(strcmp(in,"TK_ID") == 0)
    	return TK_ID;
    else if(strcmp(in,"TK_NUM") == 0)
    	return TK_NUM;
    else if(strcmp(in,"TK_OPEN") == 0)
    	return TK_OPEN;
    else if(strcmp(in,"TK_CLOSE") == 0)
    	return TK_CLOSE;
    else if(strcmp(in,"TK_SEMI") == 0)
    	return TK_SEMI;
	else if(strcmp(in,"TK_COLON") == 0)
    	return TK_COLON;
	else if(strcmp(in,"TK_EQUAL") == 0)
    	return TK_EQUAL;
	else if(strcmp(in,"TK_NOTEQUAL") == 0)
    	return TK_NOTEQUAL;
    else if(strcmp(in,"TK_LT") == 0)
    	return TK_LT;
    else if(strcmp(in,"TK_GT") == 0)
    	return TK_GT;
    else if(strcmp(in,"TK_LEQ") == 0)
    	return TK_LEQ;
    else if(strcmp(in,"TK_GEQ") == 0)
    	return TK_GEQ;
    else if(strcmp(in,"TK_COMMA") == 0)
    	return TK_COMMA;
	else if(strcmp(in,"TK_ERROR") == 0)
    	return TK_ERROR;
	else if(strcmp(in,"TK_EOF") == 0)
    	return TK_EOF;
    else if(strcmp(in,"TK_EPS") == 0)
    	return TK_EPS;
    else{
    	printf("Error Wrong String-> %s\n",in);
    	exit (1);
    }
}

int getTerminal(char *in){
	if(strcmp(in,"Program") == 0)
        return Program;
    else if(strcmp(in,"Functions") == 0)
        return Functions;
    else if(strcmp(in,"MainFun") == 0)
        return MainFun;
    else if(strcmp(in,"OtherFuns") == 0)
        return OtherFuns;
    else if(strcmp(in,"Funct") == 0)
    	return Funct;
    else if(strcmp(in,"Parameters") == 0)
    	return Parameters;
    else if(strcmp(in,"InList") == 0)
    	return InList;
    else if(strcmp(in,"IDList") == 0)
    	return IDList;
    else if(strcmp(in,"RemId") == 0)
    	return RemId;
    else if(strcmp(in,"OutId") == 0)
    	return OutId;
    else if(strcmp(in,"FunBody") == 0)
    	return FunBody;
    else if(strcmp(in,"Declarations") == 0)
    	return Declarations;
    else if(strcmp(in,"Declaration") == 0)
    	return Declaration;
    else if(strcmp(in,"OtherStatements") == 0)
    	return OtherStatements;
    else if(strcmp(in,"OtherStatementsDash") == 0)
    	return OtherStatementsDash;
    else if(strcmp(in,"Statement") == 0)
    	return Statement;
    else if(strcmp(in,"AssignmentStmt") == 0)
    	return AssignmentStmt;
    else if(strcmp(in,"AssignmentStmtDash") == 0)
    	return AssignmentStmtDash;
    else if(strcmp(in,"Expression") == 0)
    	return Expression;
    else if(strcmp(in,"FunctionCall") == 0)
    	return FunctionCall;
    else if(strcmp(in,"ConditionalStmt") == 0)
    	return ConditionalStmt;
    else if(strcmp(in,"BoolExp") == 0)
    	return BoolExp;
    else if(strcmp(in,"BoolExpDash") == 0)
    	return BoolExpDash;
    else if(strcmp(in,"RepetitiveStmt") == 0)
    	return RepetitiveStmt;
    else if(strcmp(in,"ReturnStmt") == 0)
    	return ReturnStmt;
    else if(strcmp(in,"IO_Stmt") == 0)
    	return IO_Stmt;
    else{
    	printf("Error Wrong String-> %s\n",in);
    	exit (1);
    }
}
