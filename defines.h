#ifndef DEFINES
#define DEFINES

#define _Default 0 //Default Initializer
#define _ID 1 //Match with TK_ID
#define _Program 2 //Match defaults,Replacement No
#define _MainFun 3 //Match defaults,Replacement No
#define _Funct 4 //Match with Funct,Replacement No
#define _Var 5 //Match with Declarations,Replacement Yes
#define _AssignmentStmt 6 //Match with AssignmentStmt,Replacement No
#define _ConditionalStmt 7 //Match with ConditionalStmt,Replacement No
#define _RepetitiveStmt 8 //Match with RepetitiveStmt,Replacement No
#define _ReturnStmt 9 //Match with ReturnStmt,Replacement No
#define _INStmt 10 //Match with IO_Stmt,Replacement Yes,Level 1 Lookup
#define _OUTStmt 11 //Match with IO_Stmt,Replacement Yes,Level 1 Lookup
#define _FunctionCall 12 //Match with FunctionCall,Replacement No
#define _PLUS_Expr 13 //Match with Expression,Replacement Yes,Level 1 Lookup
#define _MINUS_Expr 14 //Match with Expression,Replacement Yes,Level 1 Lookup
#define _MUL_Expr 15 //Match with Expression,Replacement Yes,Level 1 Lookup
#define _MOD_Expr 16 //Match with Expression,Replacement Yes,Level 1 Lookup
#define _DIV_Expr 17 //Match with Expression,Replacement Yes,Level 1 Lookup
#define _NUM_Expr 18 //Match with Expression,Replacement Immediate,Level 1 Lookup
#define _EqualityExp 19 //Match with BoolExp,Replacement Yes,Level 2 Lookup
#define _NonEqualityExp 20 //Match with BoolExp,Replacement Yes,Level 2 Lookup
#define _LT_Exp 21 //Match with BoolExp,Replacement Yes,Level 2 Lookup
#define _GT_Exp 22 //Match with BoolExp,Replacement Yes,Level 2 Lookup
#define _LE_Exp 23 //Match with BoolExp,Replacement Yes,Level 2 Lookup
#define _GE_Exp 24 //Match with BoolExp,Replacement Yes,Level 2 Lookup
#define _Param 25  //Match with Parameters,Replacement No
#define _ID_Exp 26 //Match with BoolExp,Replacement Yes,Level 2 Lookup

#define _EmptyStmt 27 //Remove this later
#define _NULL 28 //Remove this later
#define _NONE 29 //Remove this later
#define _RETVAR 30 //Remove this later

#define INT 0		//returnType
#define VAR 0		//returnType
#define NONE 2		//returnType

#define INS 0		//nodeType
#define FUN 1		//nodeType
#define NUM 2		//nodeType
#define EXPR 3		//nodeType
#define ASTMT 4		//nodeType
#define STMT 5		//nodeType
#define DEC 6		//nodeType
#define HEAD 7		//nodeType
#define OTHER 8		//nodeType

#endif
