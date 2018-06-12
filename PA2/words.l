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
if      {return TOKEN(IF);}
else    {return TOKEN(ELSE);}
while   {return TOKEN(WHILE);}
{int}	{SAVE_TOKEN;return INT;}
{float}	{SAVE_TOKEN;return FLOAT;}
{id}	{SAVE_TOKEN;return IDENT;}
{lnote} {;}
{bnote} {;}
{char}  {SAVE_TOKEN;return CHAR;}
"++"    {return TOKEN(TADD);}
"--"    {return TOKEN(TSUB);}
[+]		{return TOKEN(ADD);}
[-]		{return TOKEN(SUB);}
[*]		{return TOKEN(MUL);}
[/]		{return TOKEN(DIV);}
[=]		{return TOKEN(AS);}
"+="    {return TOKEN(AAS);}
"+="    {return TOKEN(SAS);}
"*="    {return TOKEN(MAS);}
"/="    {return TOKEN(DAS);}
">"    {return TOKEN(G);}
"<"    {return TOKEN(S);}
">="    {return TOKEN(GE);}
"<="    {return TOKEN(SE);}
"!="    {return TOKEN(NE);}
"=="    {return TOKEN(EQ);}
"&&"    {return TOKEN(AND);}
"||"    {return TOKEN(OR);}
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