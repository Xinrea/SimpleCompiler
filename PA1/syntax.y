%{
    #include <cstdio>
    #include <string>
%}
%union {
    int t_int;
    float t_float;
    double t_double;
    char t_char;
}
%token <t_char> CHAR
%token <t_int> INT BOOL
%token <t_float> FLOAT
%token ADD SUB MUL DIV LB RB LP RP COM AS
%type <t_double> Functions Function
%%
S : 
    |Functions {printf("end\n");}
    ;
Functions : Function
    |Functions Function
    ;
Function : TYPE ID LP PARAS RP LB STATS RB
    ;
PARAS : PARA
    |PARAS COM PARA
    ;
STATS : SRET
    |STAT STATS
    ;
STAT : EXP SEM
    |ID AS EXP SEM
    |TYPE ID AS EXP SEM
    ;
EXP : FACTOR
    |EXP ADD FACTOR
    |EXP SUB FACTOR
    ;
FACTOR : TERM
    |FACTOR MUL TERM
    |FACTOR DIV TERM
    ;
TERM : INT
    |FLOAT
    |BOOL
    |CHAR
    ;

    


%%