#ifndef PTREE
#define PTREE
#include "node.h"
extern ptrTnode pTree_init();
extern void addChild(ptrTnode cur, ptrGnode gcur);
extern void pTree_cons(ptrTnode cur);
extern void pTree_print(ptrTnode cur, int depth);
#endif
