#ifndef ABSSYN
#define ABSSYN
#include "node.h"
extern void printAST(ptrASTnode cur, int depth);
extern void buildAST(ptrTnode cur,ptrASTnode ASTp);
extern ptrASTnode initializeAST();
extern void addASTchild(ptrASTnode root, ptrASTnode child);
extern ptrASTnode createASTnode(short name,short retType,short nodeType,int index);
extern void typeCheckAST(ptrASTnode cur,int parType);
#endif
