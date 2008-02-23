#include <stdlib.h>

#include "assert.h"
#include "arbre_classe.h"
#include "anasyn.h"


function_node_t * make_function_node(size_t name_index, scope_t scope, vector_t * params, bloc_instr_node_t * block)
{
    function_node_t * r = (function_node_t *)malloc(sizeof(function_node_t));
    c_assert2(r, "malloc failed");

    
    r->params = params;
    r->ret_type = NULL;
    r->parent = NULL;
    r->name_index = name_index;
    r->block = block;
    r->scope = scope;
    r->line = yylineno;

    return r;
}

function_node_t * make_procedure_node(size_t name_index, scope_t scope, vector_t * params, bloc_instr_node_t * block)
{
    return make_function_node(name_index, scope, params, block);
}

function_node_t * make_constructor_node(scope_t scope, vector_t * params, bloc_instr_node_t * block)
{
    return make_function_node(CTOR_NAME, scope, params, block);
}


class_node_t * make_class_node(size_t name_index, tds_t * parent)
{
    class_node_t * r = (class_node_t *)malloc(sizeof(class_node_t));
    c_assert2(r, "malloc failed");

    r->super = NULL;
    r->name_index = name_index;
    r->tds = make_tds(parent);
    r->line = yylineno;

    return r;
}

field_node_t * make_field_node(scope_t scope, size_t name_index,class_node_t * parent)
{
    field_node_t * r = (field_node_t *)malloc(sizeof(field_node_t));
    c_assert2(r, "malloc failed");

    r->parent = parent;
    r->name_index = name_index;
    r->scope = scope;
    r->line = yylineno;

    return r;   
}


void free_function_node(function_node_t *e)
{
    c_assert(e);
    free_bloc_instr_node(e->block);
    free_vector(e->params, 0);
    free(e);
}


void free_class_node(class_node_t * e)
{
    c_assert(e);
    free_tds(e->tds);
    free(e);
}


void free_field_node(field_node_t * e)
{
    free(e);
}
