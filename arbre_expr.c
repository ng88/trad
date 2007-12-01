
#include <stdlib.h>

#include "assert.h"
#include "arbre_expr.h"


expr_node_t * make_expr_node(expr_node_type_t t)
{
    expr_node_t * r = malloc(sizeof(expr_node_t));
    c_assert2(r, "malloc failed");

    r->type = t;

    return r;
}


expr_node_t * make_binary_expr_node(bin_expr_node_type_t t, expr_node_t * g, expr_node_t * d)
{
    expr_node_t * r = make_expr_node(NT_BINARY);

    r->node.bin = malloc(sizeof(bin_expr_node_t));
    c_assert2( r->node.bin, "malloc failed");

    r->node.bin->type = t;
    r->node.bin->gauche = g;
    r->node.bin->droit = d;

    return r ;
}

expr_node_t * make_unary_expr_node(una_expr_node_type_t t, expr_node_t * f)
{
    expr_node_t * r = make_expr_node(NT_UNARY);

    r->node.una = malloc(sizeof(una_expr_node_t));
    c_assert2( r->node.una, "malloc failed");

    r->node.una->type = t;
    r->node.una->fils = f;

    return r ;
}

expr_node_t * make_constant_expr_node(cst_expr_node_type_t t)
{
    expr_node_t * r = make_expr_node(NT_CONST);

    r->node.cst = malloc(sizeof(cst_expr_node_t));
    c_assert2( r->node.cst, "malloc failed");

    r->node.cst->type = t;

    return r ;
}

expr_node_t * make_constant_str_expr_node(char * vstr)
{
    expr_node_t * r = make_constant_expr_node(CNT_STR);
    r->node.cst->val.vstr = vstr;
    return r;
}

expr_node_t * make_constant_idf_expr_node(char * vidf)
{
    expr_node_t * r = make_constant_expr_node(CNT_IDF);
    r->node.cst->val.vidf = vidf;
    return r;
}

expr_node_t * make_constant_int_expr_node(int vint)
{
    expr_node_t * r = make_constant_expr_node(CNT_INT);
    r->node.cst->val.vint = vint;
    return r;
}


void free_expr_node(expr_node_t * n)
{
    switch(n->type)
    {
    case NT_BINARY:
	free_binary_expr_node(n->node.bin);
	break;
    case NT_UNARY:
	free_unary_expr_node(n->node.una);
	break;
    case NT_CONST:
	free_constant_expr_node(n->node.cst);
	break;
    }
    free(n);
}


void free_binary_expr_node(bin_expr_node_t * n)
{
    free_expr_node(n->gauche);
    free_expr_node(n->droit);
}

void free_unary_expr_node(una_expr_node_t * n)
{
}

void free_constant_expr_node(cst_expr_node_t * n)
{
}
