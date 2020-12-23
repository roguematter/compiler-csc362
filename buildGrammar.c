#include<stdio.h>
#include<stdlib.h>
#include"parser.h"
#include"lexer.h"
#include"node.h"
#include"getToken_nonTerminal.h"



ptrGnode createNode(char* partIn){
	ptrGnode new;
	new = (ptrGnode)malloc(sizeof(gNode));
	if(partIn[0] == 'T'){
		new->t=terminal;
		new->id = getToken(partIn);
		new->next = NULL;
	}
	else{
		new->t=nonTerminal;
		new->id = getTerminal(partIn);
		new->next = NULL;
	}
	return new;
}


int read(char* input,char* partIn,int j){
	int k=0;
	while(input[j]!=' ' && input[j]!='\n')
		partIn[k++]=input[j++];
	partIn[k]='\0';
	if(input[j] == ' ')
		j++;
	return j;
}


ptrGnode* buildGrammar(){
	FILE *inGrammar;
	inGrammar = fopen("grammar","r");
	ptrGnode* gArray;
	ptrGnode new,next;
	char input[100];
	char* in;
	char partIn[20];
	gArray = (ptrGnode*)malloc(sizeof(ptrGnode)*(NumGRules+1));
	int i=1,j;

	in = fgets(input,100,inGrammar);
	while(in!=NULL) {

		j=0;
		j = read(input,partIn,j);
		//printf("%s\n",partIn);
		new = createNode(partIn);
		gArray[i++] = new;

		for(j = read(input,partIn,j);partIn[0]!='\0';j = read(input,partIn,j)){

			//printf("%s\n",partIn);
			next = createNode(partIn);
			new->next = next;
			new = next;
		}
		//printf("*****\n");
		in=fgets(input,100,inGrammar);
	}

	j=0;
	j = read(input,partIn,j);
	//printf("%s\n",partIn);
	new = createNode(partIn);
	gArray[i++] = new;

	for(j = read(input,partIn,j);partIn[0]!='\0';j = read(input,partIn,j)){
		//printf("%s\n",partIn);
		next = createNode(partIn);
		new->next = next;
		new = next;
	}

	//for(j=1;j<i;j++){
		//printf("%d\n",gArray[j]->id);

	//}
	return gArray;
}
