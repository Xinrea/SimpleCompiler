%option yylineno
%{
#include <stdio.h>
%}
id [_a-zA-Z][_a-zA-Z0-9]*
int [+-]?[0-9]+
float [+-]?([0-9]*\.?[0-9]+|[0-9]+\.)
lnote "//".*[\n]
bnote "/*"(.|\n)*"*/"
char '.'
%%
true	{printf("BOOL:true\n");}
false	{printf("BOOL:false\n");}
Int	{printf("T_INT\n");}
Float	{printf("T_FLOAT\n");}
Char	{printf("T_CHAR\n");}
Bool	{printf("T_BOOL\n");}
Return  {printf("RET\n");}
Print   {printf("PRINT\n");}
{int}	{printf("INT:%d\n",atoi(yytext));}
{float}	{printf("FLOAT:%f\n",atof(yytext));}
{id}	{printf("ID:%s\n",yytext);}
{lnote} {printf("LNOTE:%s",yytext);}
{bnote} {printf("BNOTE:%s\n",yytext);}
{char}  {printf("CHAR:%s\n",yytext);}
"+="    {printf("PASSIGN\n");}
"-="    {printf("MASSIGN\n");}
"*="    {printf("SASSIGN\n");}
"/="    {printf("DASSIGN\n");}
[+]		{printf("PLUS\n");}
[-]		{printf("MINUS\n");}
[*]		{printf("STAR\n");}
[/]		{printf("DIV\n");}
[=]		{printf("ASSIGN\n");}
[(]		{printf("LP\n");}
[)]		{printf("RP\n");}
[{]		{printf("LB\n");}
[}]		{printf("RB\n");}
[,]     {printf("COM\n");}
[;]		{printf("SEM\n");}
[\t]	{;}
[\n]	{;}
[ ]		{;}
.		{printf("Error at line %d: \" %s \" \n",yylineno, yytext);} 
%%

int yywrap(){
    return 1;
}