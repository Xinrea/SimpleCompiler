LDFLAGS= -L/usr/local/opt/flex/lib -L/usr/local/opt/bison/lib
CPPFLAGS= -I/usr/local/opt/flex/include

all: words.cpp syntax.tab.cpp lexer
words.cpp: words.l
	flex --outfile=words.cpp --header-file=words.hpp words.l 
syntax.tab.cpp: syntax.ypp
	bison -d -v syntax.ypp
lexer: words.cpp parse.cpp symtable.cpp
	g++ $(LDFLAGS) $(CPPFLAGS) words.cpp symtable.cpp -o lexer -std=c++11
test: unit-test-symtable.cpp
	g++ -o unit-test unit-test-symtable.cpp symtable.cpp
	./unit-test
	rm unit-test
clean:
	rm syntax.tab.cpp syntax.tab.hpp words.cpp words.hpp syntax.output
	rm lexer