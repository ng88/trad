
#include <stdlib.h>

#include "assert.h"
#include "arbre.h"


node_t * make_node(node_type_t t)
{
    node_t * r = malloc(sizeof(node_t));
    c_assert2(r, "malloc failed");

    r->type = t;

    return r;
}


node_t * make_binary_node(bin_node_type_t t, node_t * g, node_t * d)
{
    node_t * r = make_node(NT_BINARY);

    r->node.bin = malloc(sizeof(bin_node_t));
    c_assert2( r->node.bin, "malloc failed");

    r->node.bin->type = t;
    r->node.bin->gauche = g;
    r->node.bin->droit = d;

    return r ;
}

node_t * make_unary_node(una_node_type_t t, node_t * f)
{
    node_t * r = make_node(NT_UNARY);

    r->node.una = malloc(sizeof(una_node_t));
    c_assert2( r->node.una, "malloc failed");

    r->node.una->type = t;
    r->node.una->fils = f;

    return r ;
}

node_t * make_constant_node(cst_node_type_t t)
{
    node_t * r = make_node(NT_CONST);

    r->node.cst = malloc(sizeof(cst_node_t));
    c_assert2( r->node.cst, "malloc failed");

    r->node.cst->type = t;

    return r ;
}

node_t * make_constant_str_node(char * vstr)
{
    node_t * r = make_constant_node(CNT_STR);
    r->node.cst->val.vstr = vstr;
    return r;
}

node_t * make_constant_idf_node(char * vidf)
{
    node_t * r = make_constant_node(CNT_IDF);
    r->node.cst->val.vidf = vidf;
    return r;
}

node_t * make_constant_int_node(int vint)
{
    node_t * r = make_constant_node(CNT_INT);
    r->node.cst->val.vint = vint;
    return r;
}


void free_node(node_t * n)
{
    switch(n->type)
    {
    case NT_BINARY:
	free_binary_node(n->node.bin);
	break;
    case NT_UNARY:
	free_unary_node(n->node.una);
	break;
    case NT_CONST:
	free_constant_node(n->node.cst);
	break;
    }
    free(n);
}


void free_binary_node(bin_node_t * n)
{
    free_node(n->gauche);
    free_node(n->droit);
}

void free_unary_node(una_node_t * n)
{
}

void free_constant_node(cst_node_t * n)
{
}
