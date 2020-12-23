#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"parser.h"
#include"lexer.h"
#include"getToken_nonTerminal.h"

int main(){
    FILE *fiSet;
    fiSet = fopen("firstSet","r");
    short Set[26][36];
    char in[20];
    int i,j;
    for(i=0;i<26;i++){
    	for(j=0;j<36;j++){
    		Set[i][j] = 0;
    	}

    }
    while(fscanf(fiSet,"%s",in) != EOF){
        if(in[0] == 'T'){
            j = getToken(in);
        }else{
            i = getTerminal(in);
            j=-1;
        }
        if(j!=-1){
            Set[i][j] = 1;
        }
    }
    for(i=0;i<26;i++){
    	for(j=0;j<36;j++){
    		printf("%d ",Set[i][j]);
    	}
    	printf("\n");

    }

}
