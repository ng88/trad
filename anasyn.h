#ifndef ANASYN_H
#define ANASYN_H

#include "arbre.h"
#include <stdlib.h>

typedef union
{
    int vint;
    double vdouble;

    size_t index_lexique;


    expr_node_t * expr;
    instr_node_t * instr;

    param_eff_expr_node_t * params;
    rvalue_node_t * rval;

} anasyn_type_t;

#define YYSTYPE anasyn_type_t



void yyerror(char * c);
int yyparse();
int yylex();

#endif
