

%{


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

    void yyerror(char * msg);

%}

%union
{
    int vint;
    char * vstr;
}

%start prog

%token T_CST_INT
%token T_CST_STR

%token MC_CLASS
%token MC_END
%token MC_INHERIT
%token MC_PRIVATE
%token MC_PUBLIC
%token MC_INTEGER
%token MC_STRING
%token MC_SUPER
%token MC_RETURN
%token MC_NEW
%token MC_WHILE
%token MC_DO
%token MC_IF
%token MC_THEN
%token MC_ELSE
%token MC_ENDIF
%token T_IDF


%%

prog:
      T_CST_STR | T_CST_INT;


%%

void yyerror(char * msg)
{
    fprintf(stderr, "error: %s\n", msg);
}
