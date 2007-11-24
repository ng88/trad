

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
%token OP_UNARY_MINUS
%token OP_EQ
%token OP_NE
%token OP_LE
%token OP_GE
%token OP_LT
%token OP_GT
%token OP_AND
%token OP_OR
%token OP_PLUS
%token OP_MINUS
%token OP_DIV
%token OP_MUL
%token OP_AFFECT

/* Priorité des opérateurs */

%left OP_AND OP_OR

%nonassoc OP_EQ OP_NE OP_LE OP_GE OP_LT OP_GT

%left OP_PLUS OP_MINUS
%left OP_DIV OP_MUL

%nonassoc OP_UNARY_MINUS

%%

prog:
      T_CST_STR | T_CST_INT;

type:
      MC_INTEGER
    | MC_STRING
    | T_IDF
    ;

etat:
     MC_PRIVATE
   | MC_PUBLIC
   ;

exp:
     T_IDF
   | appel
   | T_CST_INT
   | T_CST_STR
   | '(' exp ')'
   | '-' exp %prec OP_UNARY_MINUS



appel:
    'T' 'O' 'D' 'O'
   ;


%%

void yyerror(char * msg)
{
    fprintf(stderr, "error: %s\n", msg);
    exit(1);
}
