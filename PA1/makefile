LDFLAGS= -L/usr/local/opt/flex/lib -L/usr/local/opt/bison/lib
CPPFLAGS= -I/usr/local/opt/flex/include

all: lex.yy.c lexer
lex.yy.c: words.l
	flex words.l
lexer: main.cpp lex.yy.c
	g++ main.cpp $(LDFLAGS) $(CPPFLAGS) -o lexer

clean:
	rm lexer
	rm lex.yy.c