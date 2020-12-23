VPATH = .

compiler : driver.o lexer.o hash.o genParseTable.o buildGrammar.o pTree.o symtable.o abssyn.o codegen.o lexer.h parser.h pTree.h symtable.h abssyn.h codegen.h
	gcc -Wall -o compiler -g -lm driver.o lexer.o hash.o buildGrammar.o genParseTable.o getToken_nonTerminal.o pTree.o symtable.o abssyn.o codegen.o
	rm -vf *.o
	
driver.o : driver.c lexer.h node.h
	gcc -Wall -c -g $<

lexer.o : lexer.c hash.o lexer.h hash.h
	gcc -Wall -c -g $<

hash.o : hash.c hash.h lexer.h
	gcc -Wall -c -g $<

genParseTable.o : genParseTable.c lexer.h parser.h node.h
	gcc -Wall -c -g $<

buildGrammar.o :buildGrammar.c getToken_nonTerminal.o lexer.h node.h
	gcc -Wall -c -g $<

getToken_nonTerminal.o : getToken_nonTerminal.c lexer.h parser.h
	gcc -Wall -c -g $<

pTree.o : pTree.c node.h lexer.h parser.h
	gcc -Wall -c -g $<

symtable.o : symtable.c node.h
	gcc -Wall -c -g $<

abssyn.o : abssyn.c symtable.o node.h lexer.h parser.h symtable.h
	gcc -Wall -c -g $<

codegen.o : codegen.c node.h
	gcc -Wall -c -g $<

clean:
	rm -vf *.o
cleanall:
	rm -vf *.o compiler
