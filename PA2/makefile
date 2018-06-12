LDFLAGS= -L/usr/local/opt/flex/lib -L/usr/local/opt/bison/lib
CPPFLAGS= -I/usr/local/opt/flex/include

all: words.cpp syntax.tab.cpp lexer
words.cpp: words.l
	flex --outfile=words.cpp --header-file=words.hpp words.l 
syntax.tab.cpp: syntax.ypp
	bison -d -v syntax.ypp
lexer: words.cpp parse.cpp
	g++ $(LDFLAGS) $(CPPFLAGS) words.cpp -o lexer -std=c++11

clean:
	rm syntax.tab.cpp syntax.tab.hpp words.cpp words.hpp syntax.output
	rm lexer