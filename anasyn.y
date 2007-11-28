

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

%start programme

%token T_CST_INT
%token T_CST_STR

%token MC_CLASS
%token MC_END
%token MC_INHERIT
%token MC_PRIVATE
%token MC_PUBLIC
%token MC_INTEGER
%token MC_STRING
%token MC_VOID
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
%token OP_BRACKET_O
%token OP_BRACKET_C
%token OP_BRACE_O
%token OP_BRACE_C
%token OP_SEMICOLON
%token OP_COMMA
%token OP_MEMBER

/* Priorité des opérateurs */

%left OP_AND OP_OR

%nonassoc OP_EQ OP_NE OP_LE OP_GE OP_LT OP_GT

%left OP_PLUS OP_MINUS
%left OP_DIV OP_MUL

%nonassoc OP_UNARY_MINUS

%%


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
   | T_CST_STR  { free($<vstr>1); }
   | OP_BRACKET_O exp OP_BRACKET_C
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
   | appel OP_MEMBER appel_membre
   ;

appel_membre:
     T_IDF OP_BRACKET_O param_eff OP_BRACKET_C
   | T_IDF
   ;

param_eff_non_vide:
     exp
   | param_eff OP_COMMA exp
   ;

param_eff:
     param_eff_non_vide
   | /* vide */
   ;

instruction:
     MC_SUPER OP_BRACKET_O param_eff OP_BRACKET_C OP_SEMICOLON
   | appel OP_SEMICOLON
   | affectation OP_SEMICOLON
   | boucle
   | cond
   | MC_RETURN OP_BRACKET_O param_eff OP_BRACKET_C OP_SEMICOLON
   ;

affectation:
     T_IDF OP_AFFECT rvalue
   ;

rvalue:
     exp
   | MC_NEW T_IDF OP_BRACKET_O param_eff OP_BRACKET_C
   ;

d_var:
     MC_VAR type liste_idf OP_SEMICOLON
   ;

liste_idf:
     T_IDF
   | liste_idf OP_COMMA T_IDF
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
     | OP_BRACE_O liste_var_non_vide liste_instruction OP_BRACE_C
     | OP_BRACE_O liste_instruction OP_BRACE_C
     ;

boucle:
       MC_WHILE exp MC_DO bloc_inst
     ;

cond:
       MC_IF exp MC_THEN bloc_inst else MC_ENDIF
     ;

else:
       MC_ELSE bloc_inst
     | /* vide */
     ;

d_var_class:
      etat type liste_idf OP_SEMICOLON
    ;

d_construct:
      etat T_IDF param bloc_inst
    ;

d_fonction:
      etat type T_IDF param bloc_inst
    ;

d_procedure:
      etat MC_VOID T_IDF param bloc_inst
    ;

param:
      OP_BRACKET_O liste_type_idf OP_BRACKET_C
    | OP_BRACKET_O OP_BRACKET_C
    ;

liste_type_idf:
      type T_IDF
    | liste_type_idf OP_COMMA type T_IDF
    ;

declaration:
      d_var_class
    | d_construct
    | d_fonction
    | d_procedure
    ;

liste_declaration_non_vide:
      declaration
    | liste_declaration_non_vide declaration
    ;

liste_declaration:
       liste_declaration_non_vide
     | /* vide */
     ;

class:
        MC_CLASS T_IDF liste_declaration MC_END
     |  MC_CLASS T_IDF MC_INHERIT T_IDF liste_declaration MC_END
     ;

programme:
       class
     | programme class
     ;

%%

void yyerror(char * msg)
{
    fprintf(stderr, "error: %s\n", msg);
    exit(1);
}
