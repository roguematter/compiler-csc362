#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include"lexer.h"
#include"hash.h"

//gives only one clash
#define PRIME 3
#define HSIZE 199


long long int hashIt(char* in){ //takes a null terminated string and returns back its hash value
	long long int val = 0;
	int i;
	for(i=0;in[i]!='\0';i++)
		val+=in[i]*((int)pow(PRIME,i));

	return val;
}

TOKEN getTokenOfSymbol(short int* hashTable, char* in,char checkTable[][30]){
	long long int hVal;
	int j;

	hVal = hashIt(in);
	if(hashTable[hVal%HSIZE] == 0 && strcmp("print\0",checkTable[hashTable[hVal%HSIZE]])!=0){
		return TK_ID;
	}
	else{
		if(strcmp(in,checkTable[hashTable[hVal%HSIZE]]) == 0)
			return hashTable[hVal%HSIZE];
		else{
			j=1;
			while(hashTable[(hVal%HSIZE)+j] != 0 && (hVal%HSIZE)+j < HSIZE){
				if(strcmp(in,checkTable[hashTable[(hVal%HSIZE)+j]]) == 0)
					return hashTable[(hVal%HSIZE)+j];
				else j++;
			}
			return TK_ID;
		}
	}
}
short int* hash(){
	FILE* input;
	input = fopen("symbolsIncode","r");
	//if(input==NULL){
	//	printf("Cannot Open ../info/symbols File\n");
	//}

	short int* hashTable;
	char in[20];
	long long int hVal;
	int i,j;
	i=0;

	hashTable = (short int*)malloc(sizeof(short int)*HSIZE);
	memset(hashTable,0,sizeof(short int)*HSIZE);

	while(fscanf(input,"%s",in)!= EOF){
		hVal = hashIt(in);
		if(hashTable[hVal%HSIZE]!=0){ //clash
			j=0;
			while(hashTable[(hVal%HSIZE)+j]!=0)
				j++;
			hashTable[(hVal%HSIZE)+j] = i;
			printf("%d\n",j);
		}
		else{
			hashTable[hVal%HSIZE] = i;
		}
		i++;
	}
	//fclose(input);
	return hashTable;
}
