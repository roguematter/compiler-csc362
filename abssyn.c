#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include "node.h"
#include "lexer.h"
#include "parser.h"
#include "defines.h"
#include "symtable.h"

extern vSymbolTable VST;
extern fSymbolTable FST;

//short returnFlag=0;
extern int scope;
int state_flag=0;
int state_flag2=0;
int cur_d = -1;

char ASTnameStr[][20]={	"Def",
					"ID",
					"Program",
					"MainFun",
					"Funct",
					"Var",
					"AssignmentStmt",
					"ConditionalStmt",
					"RepetitiveStmt",
					"ReturnStmt",
					"INStmt",
					"OUTStmt",
					"FunctionCall",
					"PLUS_Expr",
					"MINUS_Expr",
					"MUL_Expr",
					"MOD_Expr",
					"DIV_Expr",
					"NUM",
					"EqualityExp",
					"NonEqualityExp",
					"LT_Exp",
					"GT_Exp",
					"LE_Exp",
					"GE_Exp",
					"Param",
					"ID_Exp",
					"EmptyStmt",
					"NULL",//not required!!
					"TK_KEY_NONE",//not required!!
					"TK_KEY_VAR" //not required!!
					};

ptrASTnode createASTnode(short name,short retType,short nodeType,int index){
	ptrASTnode new;
	new = (ptrASTnode)malloc(sizeof(ASTnode));
	new->name = name;
	new->retType = retType;
	new->nodeType = nodeType;
	new->index = index;
	new->marked=0;
	new->reg = -1;
	new->chainHead = NULL;
	new->chainEnd = NULL;
	return new;
}

void addASTchild(ptrASTnode root, ptrASTnode child){
	if(root->chainHead==NULL) {
		root->chainHead = (ptrASTchChain)malloc(sizeof(ASTchChain));
		root->chainHead->child=child;
		root->chainHead->next=NULL;
		root->chainEnd=root->chainHead;
	}
	else{
		root->chainEnd->next=(ptrASTchChain)malloc(sizeof(ASTchChain));
		root->chainEnd->next->child=child;
		root->chainEnd->next->next=NULL;
		root->chainEnd=root->chainEnd->next;
	}
	return;
}

ptrASTnode initializeAST(){
	scope=0;
	return createASTnode(_Program,NONE,HEAD,-1);
}

void buildAST(ptrTnode cur,ptrASTnode ASTp){//pass root->child[0], ASTRoot
	int i=0;
	int index;
	ptrASTnode new=ASTp;
	char flagCont;
	/*if(cur->t==nonTerminal && cur->id==Program){
		new = createASTnode(_Program,INT,OTHER,-1);
		addASTchild(ASTp,new);
	}*/
	if(cur->t==nonTerminal && cur->id==MainFun){
		index = lookupFST("main");
		new = createASTnode(_MainFun,NONE,HEAD,index);
		addASTchild(ASTp,new);
		//returnFlag = 1;
		scope = FST[index].scope;
	}
	else if(cur->t==nonTerminal && cur->id==Funct){//get the name if function
		index=lookupFST(cur->child[1]->str);		//name of the function
		new = createASTnode(_Funct,(FST[index].outList>-1)?VAR:NONE,HEAD,index);
		addASTchild(ASTp,new);
		scope = FST[index].scope;
		state_flag=1;

		//remove this Later
		ptrASTnode new1;
		new1 = createASTnode((FST[index].outList>-1)?_RETVAR:_NONE,NONE,OTHER,-1); //Adding Return Type Node
		addASTchild(new,new1);
		//new=ASTp;
	}
	else if(cur->t==nonTerminal && cur->id==Parameters){
		new = createASTnode(_Param,NONE,DEC,-1);
		addASTchild(ASTp,new);

		//remove this Later
		if(cur->child[2]->child[0]->id == TK_KEY_NONE && cur->child[2]->child[0]->t == terminal && cur->child[6]->child[0]->id == TK_KEY_NONE && cur->child[6]->child[0]->t == terminal){
			//we have none as inList && outList
			ptrASTnode new1;
			new1 = createASTnode(_NONE,NONE,OTHER,-1); //Adding NONE as a child of Param NOT AT ALL REQUIRED!
			addASTchild(new,new1);
			new=ASTp;
		}
	}
	else if(cur->t==nonTerminal && cur->id==Declaration) {
		if(cur->child[0]->id!=TK_EPS){
			new = createASTnode(_Var,NONE,DEC,-1);
			addASTchild(ASTp,new);
		}
	}
	else if(cur->t==nonTerminal && cur->id==AssignmentStmt){
		new = createASTnode(_AssignmentStmt,NONE,ASTMT,-1);
		addASTchild(ASTp,new);
	}
	else if(cur->t==nonTerminal && cur->id==Statement && cur->child[0]->t== terminal && cur->child[0]->id==TK_SEMI){
		new = createASTnode(_EmptyStmt,NONE,STMT,-1);
		addASTchild(ASTp,new);
		ptrASTnode new1;
		new1 = createASTnode(_NULL,NONE,OTHER,-1); //Adding NULL as a child of EmptyStmt NOT AT ALL REQUIRED!
		addASTchild(new,new1);
		new=ASTp;
	}
	else if(cur->t==nonTerminal && cur->id==ConditionalStmt){
		new = createASTnode(_ConditionalStmt,NONE,STMT,-1);
		addASTchild(ASTp,new);
	}
	else if(cur->t==nonTerminal && cur->id==RepetitiveStmt){
		new = createASTnode(_RepetitiveStmt,NONE,STMT,-1);
		addASTchild(ASTp,new);
	}
	else if(cur->t==nonTerminal && cur->id==ReturnStmt){
		/*if(returnFlag == 1){
			printf("Error: The mainFunction cannot have a return statement\n");
			exit(1);
		}*/
		new = createASTnode(_ReturnStmt,NONE,STMT,-1);
		addASTchild(ASTp,new);
	}
	else if(cur->t==nonTerminal && cur->id==FunctionCall){
		index = lookupFST(cur->child[1]->str);
		new = createASTnode(_FunctionCall,(FST[index].outList>-1)?VAR:NONE,FUN,index);
		addASTchild(ASTp,new);
		state_flag=1;
	}
	else if(cur->t==nonTerminal && cur->id==IO_Stmt) {
		if(cur->child[0]->id==TK_KEY_PRINT){
			new = createASTnode(_OUTStmt,NONE,STMT,-1);
			addASTchild(ASTp,new);
		}
		else{
			new = createASTnode(_INStmt,NONE,STMT,-1);
			addASTchild(ASTp,new);
		}
	}
	else if(cur->t==nonTerminal && cur->id==Expression) {
		if(cur->child[0]->id==TK_ID){
			index = lookupVST(cur->child[0]->str,scope);
			new = createASTnode(_ID,VAR,INS,index);
			addASTchild(ASTp,new);
			state_flag=1;
		}
		else if(cur->child[0]->id==TK_NUM){
			if(atoi(cur->child[0]->str) > 32767){
				printf("OVERFLOW WARNING @ line: %d\n To continue press 'Y':",cur->child[0]->line);
				scanf("%c",&flagCont);
				if (flagCont != 'Y')
					exit(0);
				}

			new = createASTnode(_NUM_Expr,INT,NUM,atoi(cur->child[0]->str));
			addASTchild(ASTp,new);
			state_flag2=1;
		}
		else if(cur->child[0]->id==TK_KEY_PLUS){
			new = createASTnode(_PLUS_Expr,VAR,EXPR,-1);
			addASTchild(ASTp,new);
		}
		else if(cur->child[0]->id==TK_KEY_MINUS){
			new = createASTnode(_MINUS_Expr,VAR,EXPR,-1);
			addASTchild(ASTp,new);
		}
		else if(cur->child[0]->id==TK_KEY_MUL){
			new = createASTnode(_MUL_Expr,VAR,EXPR,-1);
			addASTchild(ASTp,new);
		}
		else if(cur->child[0]->id==TK_KEY_DIV){
			new = createASTnode(_DIV_Expr,VAR,EXPR,-1);
			addASTchild(ASTp,new);
		}
		else if(cur->child[0]->id==TK_KEY_MODULO){
			new = createASTnode(_MOD_Expr,VAR,EXPR,-1);
			addASTchild(ASTp,new);
		}
		else;
	}
	else if(cur->t==nonTerminal && cur->id==BoolExp) {
		if(cur->child[1]->child[0]->id==TK_EQUAL){
			new = createASTnode(_EqualityExp,VAR,EXPR,-1);
			addASTchild(ASTp,new);
		}
		else if(cur->child[1]->child[0]->id==TK_NOTEQUAL){
			new = createASTnode(_NonEqualityExp,VAR,EXPR,-1);
			addASTchild(ASTp,new);
		}
		else if(cur->child[1]->child[0]->id==TK_LT){
			new = createASTnode(_LT_Exp,VAR,EXPR,-1);
			addASTchild(ASTp,new);
		}
		else if(cur->child[1]->child[0]->id==TK_GT){
			new = createASTnode(_GT_Exp,VAR,EXPR,-1);
			addASTchild(ASTp,new);
		}
		else if(cur->child[1]->child[0]->id==TK_LEQ){
			new = createASTnode(_LE_Exp,VAR,EXPR,-1);
			addASTchild(ASTp,new);
		}
		else if(cur->child[1]->child[0]->id==TK_GEQ){
			new = createASTnode(_GE_Exp,VAR,EXPR,-1);
			addASTchild(ASTp,new);
		}
		else if(cur->child[1]->child[0]->id==TK_EPS){
			new = createASTnode(_ID_Exp,VAR,EXPR,-1);
			addASTchild(ASTp,new);
		}
		else;
	}
	else if(cur->t==terminal && cur->id==TK_ID){
		if(state_flag==1)state_flag=0;
		else{
			index = lookupVST(cur->str,scope);
			new = createASTnode(_ID,VAR,INS,index);
			addASTchild(ASTp,new);
		}
	}
	else if(cur->t==terminal && cur->id==TK_NUM){
		if(state_flag2==1)state_flag2=0;
		else{
			if(atoi(cur->str) > 32767){
				printf("OVERFLOW WARNING @ line: %d\n To continue press 'Y':",cur->line);
				scanf("%c",&flagCont);
				if (flagCont != 'Y')
					exit(0);
			}
			new = createASTnode(_NUM_Expr,INT,NUM,atoi(cur->str));
			addASTchild(ASTp,new);
		}
	}
	else;
	while(cur->child[i]!=NULL) buildAST(cur->child[i++],new);
	return;
}
void printAST(ptrASTnode cur, int depth){
	int j=depth;ptrASTchChain temp=cur->chainHead;

	if(cur_d!=depth) {
		cur_d=depth;
		printf("\n@%4d  ", depth);
		for(;j>0;j--) printf("|  ");
	}

	if(cur->nodeType > NUM){
		if(cur->name == _Funct){
			printf("%s : ID_%s\t",ASTnameStr[cur->name],FST[cur->index].funName);

		}
		else{
			printf("%s\t",ASTnameStr[cur->name]);
		}
	}

	else if(cur->nodeType==FUN)
		printf("%s : ID_%s\t",ASTnameStr[cur->name],FST[cur->index].funName);

	else if(cur->nodeType==INS)
		printf("%s_%s\t",ASTnameStr[cur->name],VST[cur->index].strPt);

	else
		printf("%s_%u\t",ASTnameStr[cur->name],cur->index);


	while(temp!=NULL) {
		printAST(temp->child, depth+1);
		temp=temp->next;
	}


	return;
}
void typeCheckAST(ptrASTnode cur,int parType){
	ptrASTchChain temp=cur->chainHead;
	if(cur->nodeType>ASTMT||cur->nodeType==INS||cur->nodeType==NUM);
	else {
		if(cur->nodeType==FUN && cur->retType==INT){
			if(parType==ASTMT);
			else {printf("\n\n\nerror: function return value not being used!\n\n\n"); exit(1);}
		}
		else if(cur->chainHead->next!=NULL && cur->chainHead->child->retType!=cur->chainHead->next->child->retType) {
			printf("\n\n\nerror: type mismatch!\n\n\n");
			exit(1);
		}
	}
	parType=cur->nodeType;
	while(temp!=NULL) {typeCheckAST(temp->child,parType);temp=temp->next;}
	return;
}
