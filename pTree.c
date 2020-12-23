#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node.h"
#include "lexer.h"
#include "parser.h"

#define _Default 0

extern TOKENTABLE tokenList[100000];

extern short pTable[NumNonTerminals][NumTokens];
extern ptrGnode *gArray;
extern char table[40][30];
int tok=0; int end=0; int d=-1;
char nonT[][20]={"Programs","Functions","MainFun","OtherFuns","Funct","Parameters","InList","IDList","RemId","OutId","FunBody","Declarations","Declaration", "OtherStatements","OtherStatementsDash","Statement","AssignmentStmt","AssignmentStmtDash","Expression","FunctionCall","ConditionalStmt","BoolExp","BoolExpDash", "RepetitiveStmt","ReturnStmt","IO_Stmt"};

ptrTnode pTree_init(){
	int i;
	ptrTnode root=(ptrTnode)malloc(sizeof(tNode));
	root->id=Program;
	root->t=nonTerminal;
	root->str[0]='\0';
	root->line=0;
	//root->ASTname=_Default;
	for(i=0;i<10;i++)
		root->child[i]=NULL;
	return root;
}

void addChild(ptrTnode cur, ptrGnode gcur){
	int i=0; int j=0;
	if(gcur==NULL) return;
	else {
		while(gcur!=NULL){
			cur->child[i]=(ptrTnode)malloc(sizeof(tNode));
			cur->child[i]->id=gcur->id;
			cur->child[i]->t=gcur->t;
			cur->child[i]->str[0]='\0';
			cur->child[i]->line=0;
			//cur->child[i]->ASTname=_Default;
			for(j=0;j<10;j++)cur->child[i]->child[j]=NULL;
			i++;gcur=gcur->next;
		}
	}
	return;
}

void pTree_cons(ptrTnode cur){
	if(tokenList[tok].tk==TK_ERROR) {printf("\n\nError in program @ line %d\n", tokenList[tok].line); exit(1);}
	ptrGnode gcur;
	int index; int i=0;
	if(end==1) return;
	if(cur->t==terminal) {
		if(cur->id!=TK_EPS){
			if(tokenList[tok].tk==TK_EOF) end=1;
			if(cur->id!=tokenList[tok].tk) {printf("\n\nError in program @ line %d\n", tokenList[tok].line); exit(1);}
			else {
				if(cur->id==TK_ID || cur->id==TK_NUM)
					strcpy(cur->str,tokenList[tok].pString);
				if(cur->id==12)
					strcpy(cur->str,"main");
				cur->line=tokenList[tok].line;tok++; return;
			}
		}
		else
			return;
	}
	else {
		index = pTable[cur->id][tokenList[tok].tk];
		if(index == 0) {printf("\n\nError in program @ line %d\n", tokenList[tok].line); exit(1);}
		gcur=gArray[index]->next;
		addChild(cur,gcur);

		while(cur->child[i]!=NULL){
			pTree_cons(cur->child[i++]);
		}
		return;
	}


}


void pTree_print(ptrTnode cur, int depth){
	int i=0; int j=depth;
	if(d!=depth) {d=depth; printf("\n@%4d  ", depth);for(;j>0;j--) printf("|  ");}
	if(cur->t==nonTerminal)printf("%s\t", nonT[cur->id]);
	else if((cur->id!=20)&&(cur->id!=12)) printf("%s\t",table[cur->id]);
	else printf("%s (%s)\t",table[cur->id],cur->str);
	while(cur->child[i]!=NULL) pTree_print(cur->child[i++], depth+1);
	return;
}
