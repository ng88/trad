

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
%token MC_VAR
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
      bloc_inst {  }
    ;

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
     // T_IDF inclu dans appel
     appel
   | T_CST_INT
   | T_CST_STR
   | '(' exp ')'
   | OP_MINUS exp %prec OP_UNARY_MINUS
   | exp OP_DIV exp
   | exp OP_MUL exp
   | exp OP_PLUS exp
   | exp OP_MINUS exp
   | exp OP_EQ exp
   | exp OP_NE exp
   | exp OP_LE exp
   | exp OP_GE exp
   | exp OP_LT exp
   | exp OP_GT exp
   | exp OP_AND exp
   | exp OP_OR exp
   ;

appel:
     appel_membre
   | appel '.' appel_membre
   ;

appel_membre:
     T_IDF '(' param_eff ')'
   | T_IDF
   ;

param_eff_non_vide:
     exp
   | param_eff ',' exp
   ;

param_eff:
     param_eff_non_vide
   | /* vide */
   ;

instruction:
     MC_SUPER '(' param_eff ')' ';'
   | appel ';'
   | affectation ';'
//   | boucle
//   | cond
   | MC_RETURN '(' param_eff ')' ';'
   ;

affectation:
     T_IDF OP_AFFECT rvalue
   ;

rvalue:
     exp
   | MC_NEW T_IDF '(' param_eff ')'
   ;

d_var:
     MC_VAR type liste_idf ';'
   ;

liste_idf:
     T_IDF
   | liste_idf ',' T_IDF
   ;

liste_var_non_vide:
     d_var
   | liste_var_non_vide d_var
   ;

liste_instruction_non_vide:
     instruction
   | liste_instruction_non_vide instruction
   ;

liste_instruction:
     liste_instruction_non_vide
   | /* vide */
   ;

bloc_inst:
     instruction
     | '{' liste_var_non_vide liste_instruction  '}'
     | '{' '}'
     ;
     


%%

void yyerror(char * msg)
{
    fprintf(stderr, "error: %s\n", msg);
    exit(1);
}
