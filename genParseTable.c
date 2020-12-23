#include<stdio.h>
#include<stdlib.h>
#include"lexer.h"
#include"parser.h"
#include"node.h"

short pTable[NumNonTerminals][NumTokens];

void genParseTable(ptrGnode* gArray){

	FILE *fiSet,*foSet;

	fiSet = fopen("firstSetInput","r");
	foSet = fopen("followSetInput","r");

	int i,j;
	short firstSet[NumNonTerminals][NumTokens];
	short followSet[NumNonTerminals][NumTokens];
	//short pTable[NumNonTerminals][NumTokens];
	short seeFollow;
	short seeFirstNext;

	for(i=0;i<NumNonTerminals;i++){
		for(j=0;j<NumTokens;j++){
			fscanf(fiSet,"%hd ",&firstSet[i][j]);
			fscanf(foSet,"%hd ",&followSet[i][j]);
			pTable[i][j] = 0;
		}
	}

	//fclose(fiSet);
	//fclose(foSet);
	ptrGnode lhs,rhs;

	for(i=1;i<=NumGRules;i++){
		lhs = gArray[i];
		rhs = lhs->next;

		do{
			seeFollow = 0;
			seeFirstNext=0;
			if(rhs->t == nonTerminal){
				for(j=0;j<NumTokens;j++){
					if(firstSet[rhs->id][j] == 1){
						if (j == TK_EPS){
							seeFirstNext = 1;
						}
						else{
							if(pTable[lhs->id][j] == 0 || pTable[lhs->id][j] == i )
								pTable[lhs->id][j] = i;
							else
								printf("Error in the PTable 1 (lhs->id,j,i) = (%d,%d,%d)\n",lhs->id,j,i);
						}
					}
				}
			}
			else if(rhs->t == terminal){
				if(rhs->id == TK_EPS){
					seeFollow = 1;
				}
				else{
					if(pTable[lhs->id][j] != 0)
						printf("Error in the PTable 2\n");
					pTable[lhs->id][rhs->id] = i;
				}
			}
			else{
				printf("STOP! SOME ERROR OCCURED in gerParseTable.c file\n");
			}
			rhs=rhs->next;
		}while(seeFirstNext==1 && rhs!=NULL && seeFollow!=1);

		if (seeFollow==1){
			for(j=0;j<NumTokens;j++){
				if(followSet[lhs->id][j]==1){
					if(pTable[lhs->id][j] != 0)
						printf("Error in the PTable 3\n");
					pTable[lhs->id][j] = i;
				}
			}
		}
	}
	/*for(i=0;i<NumNonTerminals;i++){
		for(j=0;j<NumTokens;j++){
			printf("%d ",pTable[i][j]);
		}
		printf("\n");
	}*/
}

/*int main(){
	genParseTable(buildGrammar());

	return 0;
}*/
