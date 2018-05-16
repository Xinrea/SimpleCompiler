%{
#include <stdio.h>
%}
letter [a-zA-Z]
digit [0-9]
%%
{digit}+ {printf("Int is %d\n",atoi(yytext));}
. {printf("Error at line %d: \" %s \" \n",yylineno, yytext);} 
%%

