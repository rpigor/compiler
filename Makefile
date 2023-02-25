etapa2: lex.yy.o y.tab.o hash.o
	gcc -o etapa2 lex.yy.o y.tab.o hash.o
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
	rm *.o lex.yy.c lex.yy.h y.tab.c y.tab.h etapa2
