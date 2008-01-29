#include <stdlib.h>

#include "assert.h"
#include "arbre_classe.h"



function_node_t * make_function_node(size_t name_index, scope_t scope)
{
    function_node_t * r = (function_node_t *)malloc(sizeof(function_node_t));
    c_assert2(r, "malloc failed");

    
    r->params = create_vector(2);
    r->ret_type = NULL;
    r->parent = NULL;
    r->name_index = name_index;
    r->block = NULL;
    r->scope = scope;

    return r;
}

class_node_t * make_class_node(size_t name_index, tds_t * parent)
{
    class_node_t * r = (class_node_t *)malloc(sizeof(class_node_t));
    c_assert2(r, "malloc failed");

    r->super = NULL;
    r->name_index = name_index;
    r->tds = make_tds(parent);

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


