#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include"lexer.h"
#include"parser.h"
#include"node.h"
#include"pTree.h"
#include"symtable.h"
#include"abssyn.h"
#include"codegen.h"

char table[40][30];
ptrGnode* gArray;
vSymbolTable VST;
fSymbolTable FST;
regNode regArray[4];
int main(int argc,char** argv){
	printf("##############################################\n\n\n");
	printf("\tA NONAME COMPILER FOR NONAME\n\n\t\t     BY\n\n\t[REDACTED] AND [REDACTED]\n\n   (IDs: [REDACTED] AND [REDACTED])");
	printf("\n\n\n##############################################\n\n\n");

	if(argc!=3){
		printf("\nmissing arguments\n\n\n");
		exit(1);
	}
	FILE *sym;
	sym = fopen("symbols","r");
	int i;

	for(i=0;i<41;i++){
		if (fscanf(sym,"%s",table[i]) == EOF)
		    break;
	}

	//fclose(sym);

//	printf("\n\n\n;##############################################\n\n\n");

	initializeScanner(argv[1]);

	gArray = buildGrammar();
	genParseTable(gArray);

//	printf("\n\n\n;##############################################\n\n\n");

	ptrTnode root=pTree_init();
	pTree_cons(root);
	//pTree_print(root, 0);

//	printf("\n\n\n;##############################################\n\n\n");

	initializeVST();
	initializeFST();
	record(root);

	ptrASTnode ASTroot = (ptrASTnode)initializeAST();
	buildAST(root->child[0],ASTroot);
//	printAST(ASTroot,0);
	typeCheckAST(ASTroot,8);

//	printf("\n\n\n;##############################################\n\n\n");

	FILE *outFp = fopen(argv[2],"w");
	initializeReg();
	code_generator(outFp,ASTroot,0);

//	printf("\n\n\n;##############################################\n\n\n");

//	printf("\n\n\n;##############################################\n\n\n");
	printf("   Program compiled successfully :)");
	printf("\n\n\n##############################################\n\n\n");

	return 0;
}
