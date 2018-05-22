all: lex.yy.c lexer
lex.yy.c: words.l
	flex words.l
lexer: main.cpp lex.yy.c
	g++ main.cpp -lfl -o lexer

clean:
	rm lexer
	rm lex.yy.c
