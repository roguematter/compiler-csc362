#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include "node.h"
#include "defines.h"

short int data_init=0, code_init=0;
int labelCnt=0;
char mulDivMod[3][5] = {"imul","idiv","idiv"};
int mdmIndex = 0;
extern regNode regArray[4];		//ax,bx,cx,dx
char regString[4][4] = {"ax","bx","cx","dx"};
extern vSymbolTable VST;

void initializeReg(){
	int i;
	for(i=0;i<4;i++){
		regArray[i].ASTaddr=NULL;
		regArray[i].free=1;
		regArray[i].depth=-1;
	}
}

inline int nextRegNo(){
	int i,j,leastDepth;
	for(i=0;i<4;i++){
		if(regArray[i].free == 1){
			break;
		}
	}
	j = i;
	if(i==4){
		leastDepth = regArray[0].depth;
		j = 0;
		for(i=1;i<4;i++){
			if(leastDepth > regArray[i].depth){
				j = i;
				leastDepth = regArray[i].depth;
			}
		}
	}
	return j;
}
void code_generator(FILE *outFp,ptrASTnode cur, int depth){
	ptrASTchChain temp=cur->chainHead;
	ptrASTnode lChild,rChild;
	int label;
	int i,j;
	if(cur->name==_Funct||cur->name==_FunctionCall||cur->name==_Param){
		printf("\ninvalid intermediate representation\n\n\n");
		exit (1);
	}
	if(cur->nodeType==ASTMT||cur->nodeType==STMT){
		if(code_init==0){
			code_init=1;
			fprintf(outFp,".code\r\n");


			fprintf(outFp,"\
print proc\r\n\
	mov bx,ax\r\n\
	cmp bx,0\r\n\
	je pZero\r\n\
	jg pPos\r\n\
	not bx\r\n\
	add bx,1\r\n\
	xor ax,ax\r\n\
	mov ah,02h\r\n\
	mov dl,2Dh\r\n\
	int 21h\r\n\
pPos:\r\n\
	mov ax,bx\r\n\
	mov bx,10\r\n\
	mov si,0\r\n\
itoa:\r\n\
	cmp ax,0\r\n\
	je startPrint\r\n\
	xor dx,dx\r\n\
	div bx\r\n\
	add dl,30h\r\n\
	push dx\r\n\
	inc si\r\n\
	jmp itoa\r\n\
startPrint:\r\n\
	cmp si,0\r\n\
	je donePrint\r\n\
	pop dx\r\n\
	dec si\r\n\
	mov ah,02h\r\n\
	int 21h\r\n\
	jmp startPrint\r\n\
pZero:\r\n\
	mov dl,30h\r\n\
	mov ah,02h\r\n\
	int 21h\r\n\
donePrint:\r\n\
	mov ah,02h\r\n\
	mov dl,13\r\n\
	int 21h\r\n\
	mov dl,10\r\n\
	int 21h\r\n\
	ret\r\n\
print endp\r\n");

			fprintf(outFp,"\
scan proc\r\n\
	mov dx, offset readBuf\r\n\
	mov ah,0ah\r\n\
	int 21h\r\n\
	xor cx,cx\r\n\
	mov cl,readBuf+1\r\n\
	mov si,0\r\n\
	mov bx,10\r\n\
	mov ax,0\r\n\
atoi:\r\n\
	cmp si,cx\r\n\
	je donescan\r\n\
	mul bx\r\n\
	xor dx,dx\r\n\
	mov dl,readBuf[si+2]\r\n\
	sub dx,30h\r\n\
	add ax,dx\r\n\
	inc si\r\n\
	jmp atoi\r\n\
donescan:\r\n\
	mov bx,ax\r\n\
	mov ah,02h\r\n\
	mov dl,13\r\n\
	int 21h\r\n\
	mov dl,10\r\n\
	int 21h\r\n\
	mov ax,bx\r\n\
	ret\r\n\
scan endp\r\n");

			fprintf(outFp,".startup\r\n");
		}
		if(cur->name==_ConditionalStmt||cur->name==_RepetitiveStmt){
			label = labelCnt++;
			if(cur->name==_RepetitiveStmt){
				fprintf(outFp,"label%d:\r\n",label);
				label = labelCnt++;
			}
			if(cur->chainHead->child->name==_ID_Exp){
				fprintf(outFp,"\tcmp word ptr id_%s,0\r\n",VST[cur->chainHead->child->chainHead->child->index].strPt);
				fprintf(outFp,"\tje label%d\r\n",label);
			}
			else{
				fprintf(outFp,"\tmov si,id_%s\r\n",VST[cur->chainHead->child->chainHead->child->index].strPt);
				fprintf(outFp,"\tcmp si,id_%s\r\n",VST[cur->chainHead->child->chainHead->next->child->index].strPt);
				if(cur->chainHead->child->name==_EqualityExp){
					fprintf(outFp,"\tjne label%d\r\n",label);
				}
				else if(cur->chainHead->child->name==_NonEqualityExp){
					fprintf(outFp,"\tje label%d\r\n",label);
				}
				else if(cur->chainHead->child->name==_GT_Exp){
					fprintf(outFp,"\tjng label%d\r\n",label);
				}
				else if(cur->chainHead->child->name==_LT_Exp){
					fprintf(outFp,"\tjnl label%d\r\n",label);
				}
				else if(cur->chainHead->child->name==_GE_Exp){
					fprintf(outFp,"\tjnge label%d\r\n",label);
				}
				else if(cur->chainHead->child->name==_LE_Exp){
					fprintf(outFp,"\tjnle label%d\r\n",label);
				}
			}
		}
	}
	if(cur->name == _ID)
		return;

	////////////////////Recursing/////////////////

	while(temp!=NULL) {
		code_generator(outFp,temp->child, depth+1);
		temp=temp->next;
	}

	////////////////Done Recursing/////////////////

	if(cur->name==_Var){
		if(data_init==0){
			fprintf(outFp,".model large\r\n.stack 1024\r\n.data\r\n\ttmp dw 0\r\n\treadBuf db 6, 7 dup(0)\r\n");
			data_init=1;
		}
		temp=cur->chainHead;
		while(temp!=NULL) {
			fprintf(outFp,"\tid_%s dw 0\r\n",VST[temp->child->index].strPt);
			temp=temp->next;
		}
	}
	else if(cur->name==_ConditionalStmt){
		fprintf(outFp,"label%d:\r\n",label);
	}
	else if(cur->name==_RepetitiveStmt){
		fprintf(outFp,"\tjmp label%d\r\n",label-1);
		fprintf(outFp,"label%d:\r\n",label);
	}
	else if(cur->name==_AssignmentStmt){
		lChild = cur->chainHead->child;
		rChild = cur->chainHead->next->child;
		if(rChild->name==_ID){
			fprintf(outFp,"\tmov si,id_%s\r\n",VST[rChild->index].strPt);
			fprintf(outFp,"\tmov id_%s,si\r\n",VST[lChild->index].strPt);
		}
		else if(rChild->name==_NUM_Expr){
			fprintf(outFp,"\tmov word ptr id_%s,%d\r\n",VST[lChild->index].strPt,rChild->index);
		}
		else{
			j = rChild->reg;
			if(j>=0){
				fprintf(outFp,"\tmov id_%s,%s\r\n",VST[lChild->index].strPt,regString[j]);
				regArray[j].free = 1;
				regArray[j].ASTaddr = NULL;
				regArray[j].depth = -1;
			}
			else if(j<0 && rChild->marked==1){
				fprintf(outFp,"\tpop si\r\n");
				fprintf(outFp,"\tmov id_%s,si\r\n",VST[lChild->index].strPt);
			}
		}
	}
	else if(cur->name==_PLUS_Expr||cur->name==_MINUS_Expr){
		lChild = cur->chainHead->child;
		rChild = cur->chainHead->next->child;
		if((lChild->name == _ID || lChild->name == _NUM_Expr ) && (rChild->name == _NUM_Expr || rChild->name == _ID)){
			//get a free reg and mov the value
			//mov "freeReg", VST[lChild->index].strPt
			j = nextRegNo();
			if(regArray[j].free == 1){
				regArray[j].free = 0;
			}
			else{
				(regArray[j].ASTaddr)->marked = 1;
				(regArray[j].ASTaddr)->reg = -1;
				fprintf(outFp,"\tpush %s\r\n", regString[j]);
			}
			if(lChild->name == _ID)
				fprintf(outFp,"\tmov %s,id_%s\r\n",regString[j],VST[lChild->index].strPt);
			else
				fprintf(outFp,"\tmov %s,%d\r\n",regString[j],lChild->index);
			if(rChild->name == _ID)
				fprintf(outFp,"\t%s %s,id_%s\r\n",(cur->name==_PLUS_Expr)?"add\0":"sub\0",regString[j],VST[rChild->index].strPt);
			else
				fprintf(outFp,"\t%s %s,%d\r\n",(cur->name==_PLUS_Expr)?"add\0":"sub\0",regString[j],rChild->index);
			cur->reg = j;
			regArray[j].ASTaddr = cur;
			regArray[j].depth = depth;
		}
		else if((lChild->name == _ID || lChild->name == _NUM_Expr) && (rChild->name != _ID && rChild->name != _NUM_Expr)){
			if(rChild->reg >=0){
				j = rChild->reg;
			}
			else if(rChild->reg==-1 && rChild->marked==1){

				j = nextRegNo();
				if(regArray[j].free == 1){
					regArray[j].free = 0;
					fprintf(outFp,"\tpop %s\r\n",regString[j]);
				}
				else{
					(regArray[j].ASTaddr)->marked = 1;
					(regArray[j].ASTaddr)->reg = -1;
					fprintf(outFp,"\tpop si\r\n");
					fprintf(outFp,"\tpush %s\r\n", regString[j]);
					fprintf(outFp,"\tmov %s,si\r\n", regString[j]);
				}
			}
			if(cur->name==_MINUS_Expr){
				if(lChild->name == _NUM_Expr){
					fprintf(outFp,"\tmov si,%d\r\n",lChild->index);
					fprintf(outFp,"\tsub si,%s\r\n",regString[j]);
					fprintf(outFp,"\tmov %s,si\r\n",regString[j]);
				}
				else{
					fprintf(outFp,"\tmov si,id_%s\r\n",VST[lChild->index].strPt);
					fprintf(outFp,"\tsub si,%s\r\n",regString[j]);
					fprintf(outFp,"\tmov %s,si\r\n",regString[j]);
				}
			}
			else{
				if(lChild->name == _NUM_Expr){
					fprintf(outFp,"\tadd %s,%d\r\n",regString[j],lChild->index);
				}
				else{
					fprintf(outFp,"\tadd %s,id_%s\r\n",regString[j],VST[lChild->index].strPt);
				}
			}

			cur->reg = j;
			regArray[j].ASTaddr = cur;
			regArray[j].depth = depth;
		}
		else if((rChild->name == _ID || rChild->name == _NUM_Expr) && (lChild->name != _ID && lChild->name != _NUM_Expr)){
			if(lChild->reg >=0){
				j = lChild->reg;
			}
			else if(lChild->reg==-1 && lChild->marked==1){

				j = nextRegNo();
				if(regArray[j].free == 1){
					regArray[j].free = 0;
					fprintf(outFp,"\tpop %s\r\n",regString[j]);
				}
				else{
					(regArray[j].ASTaddr)->marked = 1;
					(regArray[j].ASTaddr)->reg = -1;
					fprintf(outFp,"\tpop si\r\n");
					fprintf(outFp,"\tpush %s\r\n", regString[j]);
					fprintf(outFp,"\tmov %s,si\r\n", regString[j]);
				}
			}
			if(rChild->name == _NUM_Expr){
				fprintf(outFp,"\t%s %s,%d\r\n",(cur->name==_PLUS_Expr)?"add\0":"sub\0",regString[j],rChild->index);
			}
			else{
				fprintf(outFp,"\t%s %s,id_%s\r\n",(cur->name==_PLUS_Expr)?"add\0":"sub\0",regString[j],VST[rChild->index].strPt);
			}
			cur->reg = j;
			regArray[j].ASTaddr = cur;
			regArray[j].depth = depth;
		}
		else if(lChild->name != _ID && lChild->name != _NUM_Expr && rChild->name != _ID && rChild->name != _NUM_Expr){
			if(rChild->reg >=0 && lChild->reg >=0){
				i = rChild->reg;
				regArray[i].free = 1;
				regArray[i].ASTaddr = NULL;
				regArray[i].depth = -1;
				j = lChild->reg;
				fprintf(outFp,"\t%s %s,%s\r\n",(cur->name==_PLUS_Expr)?"add\0":"sub\0",regString[j],regString[i]);
			}
			else if((lChild->reg>=0)&&(rChild->reg==-1 && rChild->marked==1)){
				j = lChild->reg;
				fprintf(outFp,"\tpop si\r\n");
				fprintf(outFp,"\t%s %s,si\r\n",(cur->name==_PLUS_Expr)?"add\0":"sub\0",regString[j]);
			}
			else if((rChild->reg>=0)&&(lChild->reg==-1 && lChild->marked==1)){
				j = rChild->reg;
				fprintf(outFp,"\tpop si\r\n");
				if(cur->name==_MINUS_Expr){
					fprintf(outFp,"\tsub si,%s\r\n",regString[j]);
					fprintf(outFp,"\tmov %s,si\r\n",regString[j]);
				}
				else{
					fprintf(outFp,"\tadd %s,si\r\n",regString[j]);
				}
			}
			else if((rChild->reg==-1 && rChild->marked==1)&&(lChild->reg==-1 && lChild->marked==1)){
				j = nextRegNo();
				if(regArray[j].free == 1){
					regArray[j].free = 0;
					fprintf(outFp,"\tpop si\r\n");
					fprintf(outFp,"\tpop %s\r\n",regString[j]);
				}
				else{
					(regArray[j].ASTaddr)->marked = 1;
					(regArray[j].ASTaddr)->reg = -1;
					fprintf(outFp,"\tpop si\r\n");
					fprintf(outFp,"\tmov di,si\r\n");
					fprintf(outFp,"\tpop si\r\n");
					fprintf(outFp,"\tpush %s\r\n", regString[j]);
					fprintf(outFp,"\tmov %s,si\r\n", regString[j]);
					fprintf(outFp,"\tmov si,di\r\n");
				}
				fprintf(outFp,"\t%s %s,si\r\n",(cur->name==_PLUS_Expr)?"add\0":"sub\0",regString[j]);
			}
			cur->reg = j;
			regArray[j].ASTaddr = cur;
			regArray[j].depth = depth;
		}
	}



	else if(cur->name==_MUL_Expr||cur->name==_DIV_Expr||cur->name==_MOD_Expr){
		if(cur->name == _MUL_Expr)
			mdmIndex = 0;
		else if(cur->name == _DIV_Expr)
			mdmIndex = 1;
		else
			mdmIndex = 2;

		lChild = cur->chainHead->child;
		rChild = cur->chainHead->next->child;

		if((lChild->name == _ID || lChild->name == _NUM_Expr ) && (rChild->name == _NUM_Expr || rChild->name == _ID)){
			j = nextRegNo();
			if(regArray[j].free == 1){
				regArray[j].free = 0;
			}
			else{
				(regArray[j].ASTaddr)->marked = 1;
				(regArray[j].ASTaddr)->reg = -1;
				fprintf(outFp,"\tpush %s\r\n", regString[j]);
			}
			if(j!=0)
				fprintf(outFp,"\tmov %s,ax\r\n",regString[j]);

			if(lChild->name==_ID)
				fprintf(outFp,"\tmov ax,id_%s\r\n",VST[lChild->index].strPt);
			else
				fprintf(outFp,"\tmov ax,%d\r\n",lChild->index);

			if(regArray[3].free==1){
				if(mdmIndex!=0) fprintf(outFp,"\tmov dx,ax\r\n\tand dx,8000h\r\n\trol dx,1\r\n\tadd dx,0ffffh\r\n\tnot dx\r\n");
				if(rChild->name==_ID)
					fprintf(outFp,"\t%s word ptr id_%s\r\n",mulDivMod[mdmIndex],VST[rChild->index].strPt);
				else{
					fprintf(outFp,"\tmov di,%d\r\n",rChild->index);
					fprintf(outFp,"\t%s di\r\n",mulDivMod[mdmIndex]);
				}
				if(mdmIndex == 2)
					fprintf(outFp,"\tmov ax,dx\r\n");//handing mod instruction
			}
			else{
				fprintf(outFp,"\tmov si,dx\r\n");
				if(mdmIndex!=0) fprintf(outFp,"\tmov dx,ax\r\n\tand dx,8000h\r\n\trol dx,1\r\n\tadd dx,0ffffh\r\n\tnot dx\r\n");
				if(rChild->name==_ID)
					fprintf(outFp,"\t%s word ptr id_%s\r\n",mulDivMod[mdmIndex],VST[rChild->index].strPt);
				else{
					fprintf(outFp,"\tmov di,%d\r\n",rChild->index);
					fprintf(outFp,"\t%s di\r\n",mulDivMod[mdmIndex]);
				}
				if(mdmIndex == 2)
					fprintf(outFp,"\tmov ax,dx\r\n");//handing mod instruction
				fprintf(outFp,"\tmov dx,si\r\n");
			}

			if(j!=0){
				fprintf(outFp,"\tmov si,ax\r\n");
				fprintf(outFp,"\tmov ax,%s\r\n",regString[j]);
				fprintf(outFp,"\tmov %s,si\r\n",regString[j]);
			}
			cur->reg = j;
			regArray[j].ASTaddr = cur;
			regArray[j].depth = depth;
		}
		else if((lChild->name == _ID || lChild->name == _NUM_Expr) && (rChild->name != _ID && rChild->name != _NUM_Expr)){
			if(rChild->reg >=0){
				j = rChild->reg;
			}
			else if(rChild->reg==-1 && rChild->marked==1){
				j = nextRegNo();
				if(regArray[j].free == 1){
					regArray[j].free = 0;
					fprintf(outFp,"\tpop %s\r\n",regString[j]);
				}
				else{
					(regArray[j].ASTaddr)->marked = 1;
					(regArray[j].ASTaddr)->reg = -1;
					fprintf(outFp,"\tpop si\r\n");
					fprintf(outFp,"\tpush %s\r\n", regString[j]);
					fprintf(outFp,"\tmov %s,si\r\n", regString[j]);
				}
			}

			if(j==0){
				if(regArray[3].free==1){
					fprintf(outFp,"\tmov si,ax\r\n");
					if(lChild->name==_ID)
						fprintf(outFp,"\tmov ax,id_%s\r\n",VST[lChild->index].strPt);
					else
						fprintf(outFp,"\tmov ax,%d\r\n",lChild->index);
					if(mdmIndex!=0) fprintf(outFp,"\tmov dx,ax\r\n\tand dx,8000h\r\n\trol dx,1\r\n\tadd dx,0ffffh\r\n\tnot dx\r\n");
					fprintf(outFp,"\t%s si\r\n",mulDivMod[mdmIndex]);
					if(mdmIndex==2)
						fprintf(outFp,"\tmov ax,dx\r\n");
				}
				else{
					fprintf(outFp,"\tmov di,ax\r\n");
					if(lChild->name==_ID)
						fprintf(outFp,"\tmov ax,id_%s\r\n",VST[lChild->index].strPt);
					else
						fprintf(outFp,"\tmov ax,%d\r\n",lChild->index);
					fprintf(outFp,"\tmov si,dx\r\n");
					if(mdmIndex!=0) fprintf(outFp,"\tmov dx,ax\r\n\tand dx,8000h\r\n\trol dx,1\r\n\tadd dx,0ffffh\r\n\tnot dx\r\n");
					fprintf(outFp,"\t%s di\r\n",mulDivMod[mdmIndex]);
					if(mdmIndex==2)
						fprintf(outFp,"\tmov ax,dx\r\n");
					fprintf(outFp,"\tmov dx,si\r\n");
				}
			}
			else{
				if(regArray[3].free==1){
					fprintf(outFp,"\tmov si,%s\r\n",regString[j]);
					fprintf(outFp,"\tmov %s,ax\r\n",regString[j]);
					if(lChild->name==_ID)
						fprintf(outFp,"\tmov ax,id_%s\r\n",VST[lChild->index].strPt);
					else
						fprintf(outFp,"\tmov ax,%d\r\n",lChild->index);
					if(mdmIndex!=0) fprintf(outFp,"\tmov dx,ax\r\n\tand dx,8000h\r\n\trol dx,1\r\n\tadd dx,0ffffh\r\n\tnot dx\r\n");
					fprintf(outFp,"\t%s si\r\n",mulDivMod[mdmIndex]);
					if(mdmIndex==2)
						fprintf(outFp,"\tmov ax,dx\r\n");
				}
				else{
					fprintf(outFp,"\tmov di,%s\r\n",regString[j]);
					fprintf(outFp,"\tmov %s,ax\r\n",regString[j]);
					if(lChild->name==_ID)
						fprintf(outFp,"\tmov ax,id_%s\r\n",VST[lChild->index].strPt);
					else
						fprintf(outFp,"\tmov ax,%d\r\n",lChild->index);
					fprintf(outFp,"\tmov si,dx\r\n");
					if(mdmIndex!=0) fprintf(outFp,"\tmov dx,ax\r\n\tand dx,8000h\r\n\trol dx,1\r\n\tadd dx,0ffffh\r\n\tnot dx\r\n");
					fprintf(outFp,"\t%s di\r\n",mulDivMod[mdmIndex]);
					if(mdmIndex==2)
						fprintf(outFp,"\tmov ax,dx\r\n");
					fprintf(outFp,"\tmov dx,si\r\n");
				}
				fprintf(outFp,"\tmov si,ax\r\n");
				fprintf(outFp,"\tmov ax,%s\r\n",regString[j]);
				fprintf(outFp,"\tmov %s,si\r\n",regString[j]);
			}
			cur->reg = j;
			regArray[j].ASTaddr = cur;
			regArray[j].depth = depth;
		}
		else if((rChild->name == _ID || rChild->name == _NUM_Expr) && (lChild->name != _ID && lChild->name != _NUM_Expr)){
			if(lChild->reg >=0){
				j = lChild->reg;
			}
			else if(lChild->reg==-1 && lChild->marked==1){
				j = nextRegNo();
				if(regArray[j].free == 1){
					regArray[j].free = 0;
					fprintf(outFp,"\tpop %s\r\n",regString[j]);
				}
				else{
					(regArray[j].ASTaddr)->marked = 1;
					(regArray[j].ASTaddr)->reg = -1;
					fprintf(outFp,"\tpop si\r\n");
					fprintf(outFp,"\tpush %s\r\n", regString[j]);
					fprintf(outFp,"\tmov %s,si\r\n", regString[j]);
				}
			}
			if(j==0){
				if(regArray[3].free==1){
					if(mdmIndex!=0) fprintf(outFp,"\tmov dx,ax\r\n\tand dx,8000h\r\n\trol dx,1\r\n\tadd dx,0ffffh\r\n\tnot dx\r\n");
					if(rChild->name==_ID)
						fprintf(outFp,"\t%s word ptr id_%s\r\n",mulDivMod[mdmIndex],VST[rChild->index].strPt);
					else{
						fprintf(outFp,"\tmov di,%d\r\n",rChild->index);
						fprintf(outFp,"\t%s di\r\n",mulDivMod[mdmIndex]);
					}
					if(mdmIndex==2)
						fprintf(outFp,"\tmov ax,dx\r\n");
				}
				else{
					fprintf(outFp,"\tmov si,dx\r\n");
					if(mdmIndex!=0) fprintf(outFp,"\tmov dx,ax\r\n\tand dx,8000h\r\n\trol dx,1\r\n\tadd dx,0ffffh\r\n\tnot dx\r\n");
					if(rChild->name==_ID)
						fprintf(outFp,"\t%s word ptr id_%s\r\n",mulDivMod[mdmIndex],VST[rChild->index].strPt);
					else{
						fprintf(outFp,"\tmov di,%d\r\n",rChild->index);
						fprintf(outFp,"\t%s di\r\n",mulDivMod[mdmIndex]);
					}
					if(mdmIndex==2)
						fprintf(outFp,"\tmov ax,dx\r\n");
					fprintf(outFp,"\tmov dx,si\r\n");
				}
			}
			else{
				if(regArray[3].free==1){
					fprintf(outFp,"\tmov si,ax\r\n");
					fprintf(outFp,"\tmov ax,%s\r\n",regString[j]);
					if(mdmIndex!=0) fprintf(outFp,"\tmov dx,ax\r\n\tand dx,8000h\r\n\trol dx,1\r\n\tadd dx,0ffffh\r\n\tnot dx\r\n");
					if(rChild->name==_ID)
						fprintf(outFp,"\t%s word ptr id_%s\r\n",mulDivMod[mdmIndex],VST[rChild->index].strPt);
					else{
						fprintf(outFp,"\tmov di,%d\r\n",rChild->index);
						fprintf(outFp,"\t%s di\r\n",mulDivMod[mdmIndex]);
					}
					if(mdmIndex==2)
						fprintf(outFp,"\tmov ax,dx\r\n");

					fprintf(outFp,"\tmov %s,ax\r\n",regString[j]);
					fprintf(outFp,"\tmov ax,si\r\n");
				}
				else{
					fprintf(outFp,"\tmov di,ax\r\n");
					fprintf(outFp,"\tmov ax,%s\r\n",regString[j]);
					fprintf(outFp,"\tmov si,dx\r\n");

					if(mdmIndex!=0) fprintf(outFp,"\tmov dx,ax\r\n\tand dx,8000h\r\n\trol dx,1\r\n\tadd dx,0ffffh\r\n\tnot dx\r\n");
					if(rChild->name==_ID)
						fprintf(outFp,"\t%s word ptr id_%s\r\n",mulDivMod[mdmIndex],VST[rChild->index].strPt);
					else{
						fprintf(outFp,"\tmov word ptr [tmp],%d\r\n",rChild->index);
						fprintf(outFp,"\t%s word ptr [tmp]\r\n",mulDivMod[mdmIndex]);
					}
					if(mdmIndex==2)
						fprintf(outFp,"\tmov ax,dx\r\n");

					fprintf(outFp,"\tmov dx,si\r\n");
					fprintf(outFp,"\tmov %s,ax\r\n",regString[j]);
					fprintf(outFp,"\tmov ax,di\r\n");
				}
			}
			cur->reg = j;
			regArray[j].ASTaddr = cur;
			regArray[j].depth = depth;
		}
		else if(lChild->name != _ID && lChild->name != _NUM_Expr && rChild->name != _ID && rChild->name != _NUM_Expr){
			if(rChild->reg >=0 && lChild->reg >=0){
				i = rChild->reg;
				regArray[i].free = 1;
				regArray[i].ASTaddr = NULL;
				regArray[i].depth = -1;
				fprintf(outFp,"\tmov di,%s\r\n",regString[i]);
				j = lChild->reg;
			}
			else if((rChild->reg==-1 && rChild->marked==1)&&(lChild->reg==-1 && lChild->marked==1)){
				j = nextRegNo();
				if(regArray[j].free == 1){
					regArray[j].free = 0;
					fprintf(outFp,"\tpop si\r\n");
					fprintf(outFp,"\tmov di,si\r\n");
					fprintf(outFp,"\tpop %s\r\n",regString[j]);
				}
				else{
					(regArray[j].ASTaddr)->marked = 1;
					(regArray[j].ASTaddr)->reg = -1;
					fprintf(outFp,"\tpop si\r\n");
					fprintf(outFp,"\tmov di,si\r\n");
					fprintf(outFp,"\tpop si\r\n");
					fprintf(outFp,"\tpush %s\r\n", regString[j]);
					fprintf(outFp,"\tmov %s,si\r\n", regString[j]);
				}
			}
			else if((lChild->reg>=0)&&(rChild->reg==-1 && rChild->marked==1)){
				j = lChild->reg;
				fprintf(outFp,"\tpop si\r\n");
				fprintf(outFp,"\tmov di,si\r\n");
			}
			else if((rChild->reg>=0)&&(lChild->reg==-1 && lChild->marked==1)){
				j = rChild->reg;
				fprintf(outFp,"\tpop si\r\n");
				fprintf(outFp,"\tmov di,%s\r\n",regString[j]);
				fprintf(outFp,"\tmov %s,si\r\n",regString[j]);
			}
			if(j==0){
				if(regArray[3].free==1){
					if(mdmIndex!=0) fprintf(outFp,"\tmov dx,ax\r\n\tand dx,8000h\r\n\trol dx,1\r\n\tadd dx,0ffffh\r\n\tnot dx\r\n");
					fprintf(outFp,"\t%s di\r\n",mulDivMod[mdmIndex]);
					if(mdmIndex==2)
						fprintf(outFp,"\tmov ax,dx\r\n");
				}
				else{
					fprintf(outFp,"\tmov si,dx\r\n");
					if(mdmIndex!=0) fprintf(outFp,"\tmov dx,ax\r\n\tand dx,8000h\r\n\trol dx,1\r\n\tadd dx,0ffffh\r\n\tnot dx\r\n");
					fprintf(outFp,"\t%s di\r\n",mulDivMod[mdmIndex]);
					if(mdmIndex==2)
						fprintf(outFp,"\tmov ax,dx\r\n");
					fprintf(outFp,"\tmov dx,si\r\n");
				}
			}
			else{
				if(regArray[3].free==1){
					fprintf(outFp,"\tmov si,ax\r\n");
					fprintf(outFp,"\tmov ax,%s\r\n",regString[j]);
					if(mdmIndex!=0) fprintf(outFp,"\tmov dx,ax\r\n\tand dx,8000h\r\n\trol dx,1\r\n\tadd dx,0ffffh\r\n\tnot dx\r\n");
					fprintf(outFp,"\t%s di\r\n",mulDivMod[mdmIndex]);
					if(mdmIndex==2)
						fprintf(outFp,"\tmov ax,dx\r\n");
					fprintf(outFp,"\tmov %s,ax\r\n",regString[j]);
					fprintf(outFp,"\tmov ax,si\r\n");
				}
				else{
					fprintf(outFp,"\tmov si,ax\r\n");
					fprintf(outFp,"\tmov ax,%s\r\n",regString[j]);
					fprintf(outFp,"\tmov %s,si\r\n",regString[j]);
					fprintf(outFp,"\tmov si,dx\r\n");

					if(mdmIndex!=0) fprintf(outFp,"\tmov dx,ax\r\n\tand dx,8000h\r\n\trol dx,1\r\n\tadd dx,0ffffh\r\n\tnot dx\r\n");
					fprintf(outFp,"\t%s di\r\n",mulDivMod[mdmIndex]);
					if(mdmIndex==2)
						fprintf(outFp,"\tmov ax,dx\r\n");

					fprintf(outFp,"\tmov dx,si\r\n");
					fprintf(outFp,"\tmov si,%s\r\n",regString[j]);
					fprintf(outFp,"\tmov %s,ax\r\n",regString[j]);
					fprintf(outFp,"\tmov ax,si\r\n");
				}
			}
			cur->reg = j;
			regArray[j].ASTaddr = cur;
			regArray[j].depth = depth;
		}
	}




	else if(cur->name==_OUTStmt){
		fprintf(outFp,"\tpush ax\r\n\tpush bx\r\n\tpush cx\r\n\tpush dx\r\n\tpush si\r\n\tpush di\r\n\tpushf\r\n\tmov ax, id_%s\r\n\tcall print\r\n\tpopf\r\n\tpop di\r\n\tpop si\r\n\tpop dx\r\n\tpop cx\r\n\tpop bx\r\n\tpop ax\r\n",VST[cur->chainHead->child->index].strPt);
	}



	else if(cur->name==_INStmt){
		fprintf(outFp,"\tpush ax\r\n\tpush bx\r\n\tpush cx\r\n\tpush dx\r\n\tpush si\r\n\tpush di\r\n\tpushf\r\n\tcall scan\r\n\tmov id_%s, ax\r\n\tpopf\r\n\tpop di\r\n\tpop si\r\n\tpop dx\r\n\tpop cx\r\n\tpop bx\r\n\tpop ax\r\n",VST[cur->chainHead->child->index].strPt);
	}


	else if(cur->name==_MainFun){
		fprintf(outFp,".exit\r\nend\r\n");
	}
}
