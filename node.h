#ifndef NODE
#define NODE
#define SYMSIZE 10007
#define FSYMSIZE 107
#define StrSize 16

typedef struct _Gnode gNode;
typedef struct _Gnode* ptrGnode;

typedef struct _Tnode tNode;
typedef struct _Tnode* ptrTnode;

typedef struct _ASTnode ASTnode;
typedef struct _ASTnode* ptrASTnode;

typedef struct _ASTchChain ASTchChain;
typedef struct _ASTchChain* ptrASTchChain;

typedef enum{
	nonTerminal,terminal
}type;
struct _Gnode{
	short id;
	type t;
	ptrGnode next;
};

struct _Tnode{
	short id;
	type t;
	char str[StrSize];
	short line;
	//short ASTname;
	ptrTnode child[10];
};

struct _ASTchChain{
	ptrASTnode child;
	ptrASTchChain next;
};

struct _ASTnode{
	short name;
	short retType;
	short nodeType;			//tell is it ID or Function or Others
	int index;			//to index into symboltable
	short int marked;
	short int reg;
	short int evaled;
	ptrASTchChain chainHead;	//pointer to child array
	ptrASTchChain chainEnd;
};

typedef struct _VSTnode* ptrVSTnode;
typedef struct _VSTnode VSTnode;
typedef struct _FSTnode FSTnode;
typedef struct _inListNode* ptrInListNode;
typedef struct _inListNode inListNode;

struct _VSTnode{
	char strPt[StrSize];
	int memLoc;
	short scope;
};

struct _FSTnode{
	char funName[StrSize];
	ptrInListNode inList;
	int outList;
	int noInput;
	short scope;
};

typedef VSTnode vSymbolTable[SYMSIZE];
typedef FSTnode fSymbolTable[FSYMSIZE];


struct _inListNode{
	int index;
	ptrInListNode next;
};

typedef struct _QNode QNode;
typedef struct _QNode* ptrQNode;

typedef struct _regNode regNode;
struct _regNode{
	short int free;
	int depth;
	ptrASTnode ASTaddr;
};

#endif
