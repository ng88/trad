#ifndef ANASYN_H
#define ANASYN_H

#include "arbre.h"
#include "vector/vector.h"
#include <stdlib.h>

typedef struct
{
    vector_t * idf_list;
    var_type_t * type;
} idf_list_type_t;

typedef union
{
    int vint;
    double vdouble;

    size_t index_lexique;


    expr_node_t * expr;
    instr_node_t * instr;

    param_eff_expr_node_t * params;
    rvalue_node_t * rval;

    bloc_instr_node_t * bloc;

    call_expr_node_t * call;

    direct_call_expr_node_t * direct_call;

    vector_t * idf_list;

    var_type_t * type;

    idf_list_type_t idf_list_type;

    tds_t * tds;


} anasyn_type_t;

#define YYSTYPE anasyn_type_t


extern FILE *yyin, *yyout;

void yyerror(char * c);
int yyparse();
int yylex();

#endif
