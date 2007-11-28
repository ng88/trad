

%{


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "anasyn.h"

%}

%union
{
    int vint;
    char * vstr;
}

%start programme

%token T_CST_INT
%token T_CST_STR

%token FIN_FICHIER
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
      MC_INTEGER                                                  {printf("type -> MC_INTEGER \n");}
    | MC_STRING                                                   {printf("type -> MC_STRING\n");}
    | T_IDF                                                       {printf("type -> T_IDF\n");}
    ;

etat:
     MC_PRIVATE                                                   {printf("etat ->\n");}
     | MC_PUBLIC                                                  {printf("etat ->\n");} 
   ;

exp:
     // T_IDF inclu dans appel
     appel                                                         {printf("exp -> appel\n");}
   | T_CST_INT                                                     {printf("exp -> T_CST_INT\n");} 
   | T_CST_STR  { free($<vstr>1); }                                {printf("exp -> T_CST_STR\n");  free($<vstr>1);} 
   | OP_BRACKET_O exp OP_BRACKET_C                                 {printf("exp -> OP_BRACKET_O exp OP_BRACKET_C\n");} 
   | OP_MINUS exp %prec OP_UNARY_MINUS                             {printf("exp -> OP_MINUS exp %%prec OP_UNARY_MINUS\n");} 
   | exp OP_DIV exp                                                {printf("exp -> exp OP_DIV exp\n");} 
   | exp OP_MUL exp                                                {printf("exp -> exp OP_MUL exp\n");}
   | exp OP_PLUS exp                                               {printf("exp -> exp OP_PLUS exp\n");}
   | exp OP_MINUS exp                                              {printf("exp -> exp OP_MINUS exp\n");}
   | exp OP_EQ exp                                                 {printf("exp -> exp OP_EQ exp\n");}
   | exp OP_NE exp                                                 {printf("exp -> exp OP_NE exp\n");}
   | exp OP_LE exp                                                 {printf("exp -> exp OP_LE exp\n");}
   | exp OP_GE exp                                                 {printf("exp -> exp OP_GE exp\n");}
   | exp OP_LT exp                                                 {printf("exp -> exp OP_LT exp\n");}
   | exp OP_GT exp                                                 {printf("exp -> exp OP_GT exp\n");}
   | exp OP_AND exp                                                {printf("exp -> exp OP_AND exp\n");}
   | exp OP_OR exp                                                 {printf("exp -> exp OP_OR exp\n");}
   ;

appel:
     appel_membre                                                  {printf("appel -> appel_membre\n");}
   | appel OP_MEMBER appel_membre                                  {printf("appel -> appel OP_MEMBER appel_membre\n");}
   ;

appel_membre:
     T_IDF OP_BRACKET_O param_eff OP_BRACKET_C                     {printf("appel_membre -> T_IDF OP_BRACKET_O param_eff OP_BRACKET_C\n");}
   | T_IDF                                                         {printf("appel_membre -> T_IDF\n");} 
   ;

param_eff_non_vide:
     exp                                                           {printf("param_eff_non_vide -> exp\n");}
   | param_eff OP_COMMA exp                                        {printf("param_eff_non_vide -> param_eff OP_COMMA exp\n");}
   ;

param_eff:
     param_eff_non_vide                                            {printf("param_eff -> param_eff_non_vide\n");}
   | /* vide */                                                    {printf("param_eff -> \n");}
   ;

instruction:
     MC_SUPER OP_BRACKET_O param_eff OP_BRACKET_C OP_SEMICOLON     {printf("instruction -> MC_SUPER OP_BRACKET_O param_eff OP_BRACKET_C OP_SEMICOLON\n");}
   | appel OP_SEMICOLON                                            {printf("instruction -> appel OP_SEMICOLON\n");}
   | affectation OP_SEMICOLON                                      {printf("instruction -> affectation OP_SEMICOLON\n");}
   | boucle                                                        {printf("instruction -> boucle\n");}
   | cond                                                          {printf("instruction -> cond\n");}
   | MC_RETURN OP_BRACKET_O param_eff OP_BRACKET_C OP_SEMICOLON    {printf("instruction -> MC_RETURN OP_BRACKET_O param_eff OP_BRACKET_C OP_SEMICOLON\n");}
   ;

affectation:
     T_IDF OP_AFFECT rvalue                                       {printf("affectation -> T_IDF OP_AFFECT rvalue\n");}
   ;

rvalue:
     exp                                                           {printf("rvalue -> exp\n");}
   | MC_NEW T_IDF OP_BRACKET_O param_eff OP_BRACKET_C              {printf("rvalue -> MC_NEW T_IDF OP_BRACKET_O param_eff OP_BRACKET_C\n");}
   ;

d_var:
     MC_VAR type liste_idf OP_SEMICOLON                            {printf("d_var -> MC_VAR type liste_idf OP_SEMICOLON\n");}
   ;

liste_idf: 
     T_IDF                                                         {printf("liste_idf -> T_IDF\n");}
   | liste_idf OP_COMMA T_IDF                                      {printf("liste_idf -> liste_idf OP_COMMA T_IDF\n");}
   ;

liste_var_non_vide:
     d_var                                                         {printf("liste_var_non_vide -> d_var\n");}
   | liste_var_non_vide d_var                                      {printf("liste_var_non_vide -> liste_var_non_vide d_var\n");}
   ;

liste_instruction_non_vide:
     instruction                                                   {printf("liste_instruction_non_vide -> instruction\n");}
   | liste_instruction_non_vide instruction                        {printf("liste_instruction_non_vide -> liste_instruction_non_vide instruction\n");}
   ;

liste_instruction:
     liste_instruction_non_vide                                    {printf("liste_instruction -> liste_instruction_non_vide\n");}
| /* vide */                                                       {printf("liste_instruction -> \n");}
   ;

bloc_inst:
        instruction                                                {printf("bloc_inst -> instruction\n");}
     | OP_BRACE_O liste_var_non_vide liste_instruction OP_BRACE_C  {printf("bloc_inst -> OP_BRACE_O liste_var_non_vide liste_instruction OP_BRACE_C\n");}
     | OP_BRACE_O liste_instruction OP_BRACE_C                     {printf("bloc_inst -> OP_BRACE_O liste_instruction OP_BRACE_C\n");}
     ;

boucle:
       MC_WHILE exp MC_DO bloc_inst                                {printf("boucle -> MC_WHILE exp MC_DO bloc_inst\n");}
     ;

cond:
       MC_IF exp MC_THEN bloc_inst else MC_ENDIF                   {printf("cond -> MC_IF exp MC_THEN bloc_inst else MC_ENDIF\n");}
     ;

else:
       MC_ELSE bloc_inst                                           {printf("else -> MC_ELSE bloc_inst \n");}
     | /* vide */                                                  {printf("else -> \n");}
     ;

d_var_class:
      etat type liste_idf OP_SEMICOLON                             {printf("d_var_class -> etat type liste_idf OP_SEMICOLON\n");}
    ;

d_construct:
      etat T_IDF param bloc_inst                                   {printf("d_construct -> etat T_IDF param bloc_inst\n");}
    ;

d_fonction:
      etat type T_IDF param bloc_inst                              {printf("d_fonction -> etat type T_IDF param bloc_inst\n");}
    ;

d_procedure:
      etat MC_VOID T_IDF param bloc_inst                           {printf("d_procedure -> etat MC_VOID T_IDF param bloc_inst\n");}
    ;

param:
      OP_BRACKET_O liste_type_idf OP_BRACKET_C                     {printf("param -> OP_BRACKET_O liste_type_idf OP_BRACKET_C\n");}
    | OP_BRACKET_O OP_BRACKET_C                                    {printf("param -> OP_BRACKET_O OP_BRACKET_C\n");}
    ;

liste_type_idf: 
      type T_IDF                                                   {printf("liste_type_idf -> type T_IDF\n");}
    | liste_type_idf OP_COMMA type T_IDF                           {printf("liste_type_idf -> liste_type_idf OP_COMMA type T_IDF\n");}
    ;

declaration:
      d_var_class                                                  {printf("declaration -> d_var_class\n");}
    | d_construct                                                  {printf("declaration -> d_construct\n");}
    | d_fonction                                                   {printf("declaration -> d_fonction\n");}
    | d_procedure                                                  {printf("declaration -> d_procedure\n");}
    ;

liste_declaration_non_vide:
      declaration                                                  {printf("liste_declaration_non_vide -> declaration\n");}
    | liste_declaration_non_vide declaration                       {printf("liste_declaration_non_vide -> liste_declaration_non_vide declaration\n");}
    ;

liste_declaration:
       liste_declaration_non_vide                                  {printf("liste_declaration -> liste_declaration_non_vide\n");}
     | /* vide */                                                  {printf("liste_declaration -> \n");} 
     ;

class:
        MC_CLASS T_IDF liste_declaration MC_END                     {printf("class -> MC_CLASS T_IDF liste_declaration MC_END\n");}
     |  MC_CLASS T_IDF MC_INHERIT T_IDF liste_declaration MC_END    {printf("class -> MC_CLASS T_IDF MC_INHERIT T_IDF liste_declaration MC_END\n");}
     ;

l_class:
	l_class class                                               {printf("l_class -> l_class class\n");}
     |  class                                                       {printf("l_class -> class\n");}
     ;

programme:
	l_class FIN_FICHIER                                         {printf("programme -> l_class FIN_FICHIER\n");return 0;}
     ;

%%

void yyerror(char * msg)
{
    fprintf(stderr, "error: %s\n", msg);
    exit(1);
}
