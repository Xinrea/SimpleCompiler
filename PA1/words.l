%option yylineno
%option noyywrap
%{
#include <cstdio>
#include <string>
#include "syntax.tab.cpp"
#include "node.h"
#define SAVE_TOKEN (yylval.string=new std::string(yytext,yyleng))
#define TOKEN(t) (yylval.token=t)
%}
id [_a-zA-Z][_a-zA-Z0-9]*
int [+-]?[0-9]+
float [+-]?([0-9]*\.?[0-9]+|[0-9]+\.)
lnote "//".*[\n]
bnote "/*"(.|\n)*"*/"
char '.'
%%
true	{SAVE_TOKEN;return BOOL;}
false	{SAVE_TOKEN;return BOOL;}
Int	    {return TOKEN(TYPE);}
Float	{return TOKEN(TYPE);}
Char	{return TOKEN(TYPE);}
Bool	{return TOKEN(TYPE);}
Return  {return TOKEN(SRET);}
{int}	{SAVE_TOKEN;return INT;}
{float}	{SAVE_TOKEN;return FLOAT;}
{id}	{SAVE_TOKEN;return IDENT;}
{lnote} {;}
{bnote} {;}
{char}  {SAVE_TOKEN;return CHAR;}
"+="    {printf("PASSIGN\n");}
"-="    {printf("MASSIGN\n");}
"*="    {printf("SASSIGN\n");}
"/="    {printf("DASSIGN\n");}
[+]		{return TOKEN(ADD);}
[-]		{return TOKEN(SUB);}
[*]		{return TOKEN(MUL);}
[/]		{return TOKEN(DIV);}
[=]		{return TOKEN(AS);}
[(]		{return TOKEN(LP);}
[)]		{return TOKEN(RP);}
[{]		{return TOKEN(LB);}
[}]		{return TOKEN(RB);}
[,]     {return TOKEN(COM);}
[;]		{return TOKEN(SEM);}
[\t]	{;}
[\n]	{;}
[ ]		{;}
.		{printf("Error at line %d: \" %s \" \n",yylineno, yytext);} 
%%