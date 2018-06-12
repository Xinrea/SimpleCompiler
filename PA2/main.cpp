#include "syntax.tab.h"

#include <iostream>
using namespace std;

extern FILE *yyin;

int main(int argc, char** argv){
	yyin = fopen(argv[1],"r");
	if(!yyin)return 0;
	yyparse();
	return 0;
}
