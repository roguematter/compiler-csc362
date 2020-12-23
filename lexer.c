#include<stdio.h>
#include"lexer.h"
#include"hash.h"
#include<stdlib.h>
#include<string.h>
#define MAX 10000


char BUFF1[MAX],BUFF2[MAX];
int pointBf1, pointBf2,lineNum=1,colNum=1,bufFlag=1;
TOKENTABLE tokenList[100000];
FILE *input;
char checkTable[40][30];
extern char table[40][30];
short int* hashTable;
char readIn[30];
short specialDecF = 0;



void refreshBuff(){
	if (bufFlag == 1)
		fread((void*)BUFF1,1,MAX,input);

	else
		fread((void*)BUFF2,1,MAX,input);
	return;
}

inline void incrPt(){
	colNum++;
	if(pointBf1 < MAX-1 && bufFlag == 1){
		pointBf1++;
		specialDecF = 0;
	}
	else if(pointBf2 < MAX-1 && bufFlag == 2){
		pointBf2++;
		specialDecF = 0;
	}
	else if (pointBf1 == MAX-1 && bufFlag == 1){
		bufFlag = 2;
		if(specialDecF == 0)refreshBuff();
		pointBf2 = 0;
	}
	else if (pointBf2 == MAX-1 && bufFlag == 2){
		bufFlag = 1;
		if(specialDecF == 0)refreshBuff();
		pointBf1 = 0;
	}
	return;
}

inline void decrPt(){
	colNum--;
	if(pointBf1 > 0 && bufFlag == 1){
		pointBf1--;
		if(BUFF1[pointBf1] == '\n')
			lineNum--;
	}
	else if(pointBf2 > 0 && bufFlag == 2){
		pointBf2--;
		if(BUFF2[pointBf2] == '\n')
			lineNum--;
	}
	else if(pointBf1 == 0 && bufFlag == 1){
		bufFlag = 2;
		pointBf2 = MAX-1;
		if(BUFF2[pointBf2] == '\n')
			lineNum--;
		specialDecF =1;
	}
	else if(pointBf2 == 0 && bufFlag == 2){
		bufFlag = 1;
		pointBf1 = MAX-1;
		if(BUFF1[pointBf1] == '\n')
			lineNum--;
		specialDecF = 1;
	}

	return;
}

inline char readBuf(){
	char c;
	if(bufFlag == 1)
		c = BUFF1[pointBf1];
	else
		c = BUFF2[pointBf2];

	incrPt();
	if (c == '\n'){
		lineNum++;
		colNum=1;
	}
	return c;
}

int isNumb(char c){
	if (c>=48 && c<=57)
		return 1;
	else
		return 0;
}

int isAlpha(char c){
	if ((c>=65 && c<=90)||(c>=97 && c<=122))
		return 1;
	else
		return 0;
}
int isDelimiter(char c){
	if (c == ' ' ||c == '\n' ||c == '\t')
		return 1;
	else
		return 0;
}

int isOtherTok(char c){
    if (c == '(' || c == ')' || c == '=' || c == '!' || c == '<' ||c == '>'  || c == ',' || c == ';' || c == ':')
        return 1;
    else
        return 0;
}
TOKEN nextToken(){
	int i=0;
	memset(readIn,'\0',sizeof(char)*30);
	readIn[i] = readBuf();
	if (readIn[i] == EOF || readIn[i] == '\0')
		return TK_EOF;
	else if(isDelimiter(readIn[i]) == 1)
		return nextToken();
	else if(readIn[i] == '(')
		return TK_OPEN;
	else if (readIn[i] == ')')
		return TK_CLOSE;
	else if (readIn[i] == ';')
		return TK_SEMI;
	else if (readIn[i] == '=')
		return TK_EQUAL;
	else if (readIn[i] == '!'){
		readIn[++i] = readBuf();
		if(readIn[i] == '=')
			return TK_NOTEQUAL;
		else
			printf("Syntax Error at line %d, column %d, \"Equal\" symbol missing\n",lineNum,colNum);
			return TK_ERROR;
	}
	else if (readIn[i] == ')')
		return TK_CLOSE;
	else if (readIn[i] == '<'){
		readIn[++i] = readBuf();
		if (readIn[i] == '=')
			return TK_LEQ;
		else{
			decrPt();
			i--;
			return TK_LT;
		}
	}
	else if (readIn[i] == '>'){
		readIn[++i] = readBuf();
		if (readIn[i] == '=')
			return TK_GEQ;
		else{
			decrPt();
			i--;
			return TK_GT;
		}
	}
	else if (readIn[i] == ',')
		return TK_COMMA;
	else if (readIn[i] == ':')
	    return TK_COLON;

	//now check if it is digit....
	else if (isNumb(readIn[i]) == 1){
		for(readIn[++i] = readBuf(); isNumb(readIn[i])==1; readIn[++i] = readBuf());

		if (isDelimiter(readIn[i]) == 1 || isOtherTok(readIn[i]) == 1) {
			decrPt();
			readIn[i] = '\0';
			return TK_NUM;
		}
		printf("Some Number has non-numeric characters within it @ line %d, column %d\n",lineNum,colNum);
		return TK_ERROR;
	}
	//now check if is reserved or non-reserver identifier

	else if (isAlpha(readIn[i]) == 1){
		for(readIn[++i] = readBuf(); isNumb(readIn[i])==1 || isAlpha(readIn[i]) ==1; readIn[++i] = readBuf());
			if (isDelimiter(readIn[i]) == 1 || isOtherTok(readIn[i]) == 1 ){
				readIn[i] = '\0';
				decrPt();
				return getTokenOfSymbol(hashTable,readIn,checkTable);
				/*if (strcmp("print",readIn) == 0)
					return TK_KEY_PRINT;
				else if (strcmp("get",readIn) == 0)
					return TK_KEY_GET;
				else if (strcmp("return",readIn) == 0)
					return TK_KEY_RETURN;
				else if (strcmp("begin",readIn) == 0)
					return TK_KEY_BEGIN;
				else if (strcmp("end",readIn) == 0)
					return TK_KEY_END;
				else if (strcmp("while",readIn) == 0)
					return TK_KEY_WHILE;
				else if (strcmp("if",readIn) == 0)
					return TK_KEY_IF;
				else if (strcmp("fun",readIn) == 0)
					return TK_KEY_FUN;
				else if (strcmp("var",readIn) == 0)
					return TK_KEY_VAR;
				else if (strcmp("none",readIn) == 0)
					return TK_KEY_NONE;
				else if (strcmp("in",readIn) == 0)
					return TK_KEY_IN;
				else if (strcmp("out",readIn) == 0)
					return TK_KEY_OUT;
				else if (strcmp("main",readIn) == 0)
					return TK_KEY_MAIN;
				else if (strcmp("let",readIn) == 0)
					return TK_KEY_LET;
				else if (strcmp("be",readIn) == 0)
					return TK_KEY_BE;
				else if (strcmp("plus",readIn) == 0)
					return TK_KEY_PLUS;
				else if (strcmp("minus",readIn) == 0)
					return TK_KEY_MINUS;
				else if (strcmp("mul",readIn) == 0)
					return TK_KEY_MUL;
				else if (strcmp("div",readIn) == 0)
					return TK_KEY_DIV;
				else if (strcmp("mod",readIn) == 0)
					return TK_KEY_MODULO;
				else
					return TK_ID;*/

			}
			else{
				printf("Wrong identifier format is used @ line %d, column %d\n",lineNum,colNum);
				return TK_ERROR;
			}
	}
	else {
		printf("Unidentified error @ line %d, column %d\n",lineNum,colNum);
		return TK_ERROR;
	}
}

void printTokenStream(FILE* f){
	int i=0;
	TOKEN t;
	char *string;
	for(t= nextToken();t!= TK_EOF;t=nextToken(),i++){
		//printf("printTokenStream:-> Got token no.%d\n",i);
		if(t == TK_ERROR)
			exit(1);
		tokenList[i].tk = t;
		tokenList[i].line = lineNum;
		if(t == TK_ID || t == TK_NUM){
			string = (char*)malloc(sizeof(char)*50);
			strcpy(string,readIn);
			tokenList[i].pString = string;
		}
		else
			tokenList[i].pString = NULL;

	/*	if(tokenList[i].pString!=NULL)
			printf("token, line, string-> %d, %d, %s\n",t,lineNum,string);
		else
			printf("token, line,-> %d, %d\n",t,lineNum);
	*/
	}
	tokenList[i].tk=t;
	return;
}

void initializeScanner(char *filename){
	FILE *symbolIn;
	input = fopen(filename,"r");
	if(input == NULL) printf("%s file didnot open\n",filename);

	pointBf1 = 0;
	bufFlag = 1;
	refreshBuff();
	hashTable=hash();
	int i;
	i=0;
	symbolIn = fopen("symbolsIncode","r");
	while(fscanf(symbolIn,"%s",checkTable[i++])!=EOF);
	//fclose(symbolIn);
	//if(i == 0) printf("file suuccessfully closed\n");
	//else printf("error No. %d\n",i);

	//printf("initializeScanner:-> calling printTokenStream\n");
	printTokenStream(input);
	//fclose(input);
}
