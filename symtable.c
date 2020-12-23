#define FUNCT 4
#define MAIN 12
#define TKID 20
#define IN 10
#define OUT 11
#define NONE 9
#define VAR 8
#define EPS 35
#define FUN 7
#define RET 24

#include"hash.h"
#include "node.h"
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

short scope = 0;
short state = 0;
int FSTIndex = -1;
int funReturns = 0;

int IDEater = 0;
char eaterFun[StrSize]="";
int eaterLine = 0;
char curFun[StrSize]="";

extern vSymbolTable VST;
extern fSymbolTable FST;

void initializeVST(){
	int i;
	for(i=0;i<SYMSIZE;i++){
		VST[i].strPt[0]='\0';
		VST[i].memLoc = -1;
		VST[i].scope = -1;
	}
}
void initializeFST(){
	int i;
	for(i=0;i<FSYMSIZE;i++){
		FST[i].funName[0] = '\0';
		FST[i].inList = NULL;
		FST[i].outList = -1;
		FST[i].noInput = -1;
		FST[i].scope = -1;
	}
}

int lookupVST(char* strPt,short scp){
	int hashVal;
	hashVal = hashIt(strPt)%SYMSIZE;
	int i=0;
	//commented code is for variable name and funciton name clashing!
	/*if(scp<0){
		while(i<SYMSIZE){
			hashVal =  (hashVal+ (int) pow(i++,2))% SYMSIZE;
			if(VST[hashVal].strPt[0] == '\0')
				return -1;
			else if(strcmp (VST[hashVal].strPt,strPt) == 0)	return hashVal;
		}
	}
	else{*/
	while(i<SYMSIZE){
		hashVal =  (hashVal+ (int) pow(i++,2))% SYMSIZE;
		if(VST[hashVal].strPt[0] == '\0')
			return -1;
		else if(strcmp (VST[hashVal].strPt,strPt) == 0 && VST[hashVal].scope == scp)	return hashVal;
	}
	//}
	return -1;
}
int lookupFST(char* funName){
	int hashVal;
	hashVal = hashIt(funName)%FSYMSIZE;
	int i=0;
	while(i<FSYMSIZE){
		hashVal =  (hashVal+ (int) pow(i++,2))% SYMSIZE;
		if(FST[hashVal].funName[0] == '\0')
			return -1;
		else if(strcmp (FST[hashVal].funName,funName) == 0)
			return hashVal;
	}
	return -1;
}

int insertVST(char* strPt,int memloc,short scp){
//	if(lookupFST(strPt,FST)>-1) {return -1;}
	int hashVal;
	hashVal = hashIt(strPt)%SYMSIZE;
	int i=0;
	while(i<SYMSIZE){
		hashVal =  (hashVal+ (int) pow(i++,2))% SYMSIZE;
		if(VST[hashVal].strPt[0] =='\0'){
			strcpy(VST[hashVal].strPt,strPt);
			VST[hashVal].memLoc = 0;
			VST[hashVal].scope = scp;
			return hashVal;
		}
		else if(strcmp(strPt,VST[hashVal].strPt) == 0 && VST[hashVal].scope==scp){
			return -1;
		}
	}
	printf("Some SYMSIZE error!!!");
	exit(1);

}
int insertFST(char* funName,short scp){
//	if(lookupVST(funName,-1,VST)>-1) {return -1;}
	int hashVal;
	hashVal = hashIt(funName)%FSYMSIZE;
	int i=0;
	while(i<FSYMSIZE){
		hashVal =  (hashVal+ (int) pow(i++,2))% FSYMSIZE;
		if(FST[hashVal].funName[0] == '\0'){
			strcpy(FST[hashVal].funName,funName);
			FST[hashVal].inList = NULL;
			FST[hashVal].outList = -1;
			FST[hashVal].noInput = 0;
			FST[hashVal].scope = scp;
			return hashVal;
		}
		else if(strcmp(funName,FST[hashVal].funName)==0 ){
			return -1;
		}
	}
	printf("Some FSYMSIZE error!!!");
	exit(1);
}

void insertIOList(int VSTIndex, int FSTIndex){
	ptrInListNode cur = FST[FSTIndex].inList;
	if(FST[FSTIndex].inList==NULL){
		FST[FSTIndex].inList=(ptrInListNode)malloc(sizeof(inListNode));
		FST[FSTIndex].inList->index=VSTIndex;
		FST[FSTIndex].inList->next=NULL;
		FST[FSTIndex].noInput++;
	}
	else{
		while(cur->next!=NULL) cur=cur->next;
		cur->next=(ptrInListNode)malloc(sizeof(inListNode));
		cur->next->index=VSTIndex;
		cur->next->next=NULL;
		FST[FSTIndex].noInput++;
	}
}
void record(ptrTnode cur){
	int i=0;int VSTIndex=-1;int tmp_index=-1;

	if(state==0 && cur->t==0 && cur->id==FUNCT) {
		state=1;scope++;
		if(funReturns == 1){
			printf("\n\nThe function %s doesn't have a return statement at all\n\n",curFun);
			exit(1);
		}

	}

	else if(state==0 && cur->t==1 && cur->id==MAIN) {
		scope++;FSTIndex=insertFST(cur->str,scope);
		if(funReturns == 1){
			printf("\n\nThe function %s doesn't have a return statement at all\n\n",curFun);
			exit(1);
		}
		strcpy(curFun,cur->str);
	}

	else if(state==1 && cur->t==1 && cur->id==TKID) {
		FSTIndex=insertFST(cur->str,scope);
		if(FSTIndex<0) {printf("error: re-declaration of/as function '%s' @line %d\n\n",cur->str,cur->line); exit(1);}; state=0;
		strcpy(curFun,cur->str);
	}

	else if(state==0 && cur->t==1 && cur->id==IN) state=2;

	else if(state==2 && cur->t==1 && cur->id==TKID) {
		VSTIndex=insertVST(cur->str,0,scope);
		if(VSTIndex<0) {
			printf("error: re-declaration of/as variable '%s' @line %d\n\n",cur->str,cur->line);
			exit(1);
		}
		insertIOList(VSTIndex,FSTIndex);
	}

	else if(state==2 && cur->t==1 && cur->id==OUT) state=3;

	else if(state==3 && cur->t==1 && cur->id==TKID) {
		FST[FSTIndex].outList=insertVST(cur->str,0,scope);
		if(FST[FSTIndex].outList<0) {
			printf("error: re-declaration of/as variable '%s' @line %d\n\n",cur->str,cur->line);
			exit(1);
		}
		state=0;
		funReturns =1;//to check that the function should contain a return statement
	}

	else if(state==3 && cur->t==1 && cur->id==NONE) state=0;

	else if(state==0 && cur->t==1 && cur->id==VAR) state=4;

	else if(state==4 && cur->t==1 && cur->id==TKID) {
		if(insertVST(cur->str,0,scope)<0) {
			printf("error: re-declaration of/as variable '%s' @line %d\n\n",cur->str,cur->line);
			exit(1);
		}
	}

	else if(state==4 && cur->t==1 && cur->id==EPS) state=0;

	else if(state==0 && cur->t==1 && cur->id==FUN) state=5;

	else if(state==5 && cur->t==1 && cur->id==TKID) {
		tmp_index=lookupFST(cur->str);
		if(tmp_index<0){
			printf("error: undeclared function '%s' @line %d\n\n",cur->str,cur->line);
			exit(1);
		}
		else if(FST[tmp_index].scope==scope){
			printf("error: recursion not allowed in function '%s' @line %d\n\n",cur->str,cur->line);
			exit(1);
		}
		else {
			IDEater=FST[tmp_index].noInput;strcpy(eaterFun,cur->str);eaterLine=cur->line;
			state=6;
		}//+((FST[tmp_index].outList==-1)?0:1)
	}

	else if(state==6 && cur->t==1 && cur->id==TKID)--IDEater;

	else if(state==6 && cur->t==1 && cur->id==EPS) {
		if(IDEater!=0){
			printf("error: unexpected number of variables passed in call for function '%s' @line %d\n\n",eaterFun,eaterLine);
			exit(1);
		}
		else {eaterLine=0;eaterFun[0]='\0';IDEater=0;state=0;}
	}

	else if(state==0 && cur->t==1 && cur->id==TKID) {
		if(lookupVST(cur->str,scope)<0){
			printf("error: undeclared variable '%s' @line %d\n\n",cur->str,cur->line); exit(1);
		}
	}

	else if(state==0 && cur->t==0 && cur->id==RET) {
		if(FST[lookupFST(curFun)].outList==-1){
			printf("error: return statement not allowed in function '%s' @line %d\n\n",curFun,cur->child[1]->line);
			exit(1);
		}
		else{
			funReturns = 0; //checked that the funciton returned properly.
			if(strcmp(VST[FST[lookupFST(curFun)].outList].strPt,cur->child[1]->str)!=0){
				printf("error: return variable mismatch in function '%s' @line %d expected \'%s\'\n\n",
				curFun,cur->child[1]->line,VST[FST[lookupFST(curFun)].outList].strPt);
				exit(1);
			}
		}
	}

	else;

	while(cur->child[i]!=NULL) record(cur->child[i++]);
	return;
}
