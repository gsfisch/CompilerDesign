#
# UFRGS - Compiladores B - Guilherme Fisch - 2023/2 - Etapa 5
#

# Linkagem
etapa5: main.o y.tab.o lex.yy.o hash.o ast.o semantic.o tacs.o
	clang main.o lex.yy.o y.tab.o hash.o ast.c semantic.o tacs.o -o etapa5


# Compilação dos fontes
main.o: main.c
	clang -c main.c

semantic.o: semantic.c
	clang -c semantic.c

hash.o: hash.c
	clang -c hash.c

ast.o: ast.c
	clang -c ast.c

y.tab.o: y.tab.c
	clang -c y.tab.c

lex.yy.o: lex.yy.c
	clang -c lex.yy.c

tacs.o: tacs.c
	clang -c tacs.c


# YACC
y.tab.c: parser.y
	yacc -d parser.y


# FLEX
lex.yy.c: scanner.l
	lex scanner.l


clean:
	rm lex.yy.c y.tab.c y.tab.h *.o etapa5
