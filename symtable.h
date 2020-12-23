#ifndef SYMTABLE
#define SYMTABLE
#include "node.h"
extern void record(ptrTnode cur);
extern void insertIOList(int VSTIndex, int FSTIndex);
extern int lookupFST(char* funName);
extern int lookupVST(char* strPt,short scp);
extern int insertFST(char* funName,short scp);
extern int insertVST(char* strPt,int memloc,short scp);
extern void initializeFST();
extern void initializeVST();
#endif
