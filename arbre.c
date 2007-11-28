
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


void free_node(node_t * n)
{
    switch(n->type)
    {
    case NT_BINARY:
	free_binary_node(n->node.bin);
	break;
    case NT_UNARY:
	break;
    case NT_CONST:
	break;
    }
    free(n);
}


void free_binary_node(bin_node_t * n)
{
    free_node(n->gauche);
    free_node(n->droit);
}
