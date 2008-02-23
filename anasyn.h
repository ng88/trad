#ifndef ANASYN_H
#define ANASYN_H

#include "arbre.h"
#include "error.h"
#include "vector/vector.h"
#include "stack/stack.h"
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

    vector_t * type_list;

    var_type_t * type;

    idf_list_type_t idf_list_type;

    tds_t * tds;

    class_node_t * mclass;
    function_node_t * mfn;
    field_node_t * mfl;

    scope_t scope;

} anasyn_type_t;

#define YYSTYPE anasyn_type_t


extern FILE *yyin, *yyout;
extern lexique_t * c_lexique;
extern stack_t * block_stack;

tds_t * get_tds();
function_node_t * get_main_function();

void yyerror(char * c);
int yyparse();
int yylex();

void yy_m_init();
void yy_m_free();


extern int yylineno;

enum { MAX_IDF_LEN = 20 };

#endif
