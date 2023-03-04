etapa5: lex.yy.o y.tab.o hash.o ast.o semantic.o tac.o main.o
	gcc -o etapa5 lex.yy.o y.tab.o hash.o ast.o semantic.o tac.o main.o
main.o: main.c
	gcc -c main.c
tac.o: tac.c
	gcc -c tac.c
semantic.o: semantic.c
	gcc -c semantic.c
ast.o: ast.c
	gcc -c ast.c
hash.o: hash.c
	gcc -c hash.c
lex.yy.o: lex.yy.c y.tab.h
	gcc -c lex.yy.c
lex.yy.c: scanner.l
	flex --header-file=lex.yy.h scanner.l
y.tab.o: y.tab.c
	gcc -c y.tab.c
y.tab.c y.tab.h: parser.y
	yacc -d parser.y
clean:
	rm *.o lex.yy.c lex.yy.h y.tab.c y.tab.h etapa5
