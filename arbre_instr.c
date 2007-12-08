#include <stdlib.h>

#include "assert.h"
#include "arbre_instr.h"
#include "lexique.h"

instr_node_t * make_instr_node(instr_node_type_t t)
{
    instr_node_t * r = (instr_node_t *)malloc(sizeof(instr_node_t));
    c_assert2(r, "malloc failed");

    r->type = t;

    return r;
}

instr_node_t * make_loop_instr_node(expr_node_t * c, bloc_instr_node_t * b)
{
    c_assert(c && b);

    instr_node_t * r = make_instr_node(INT_LOOP);
    r->node.loop->cond = c;
    r->node.loop->body = b;

    return r;
}

instr_node_t * make_cond_instr_node(expr_node_t * c, bloc_instr_node_t * bt,
				    bloc_instr_node_t * bf)
{
    c_assert(c && bt);

    instr_node_t * r = make_instr_node(INT_COND);
    r->node.cond->cond = c;
    r->node.cond->btrue = bt;
    r->node.cond->bfalse = bf;

    return r;
}

instr_node_t * make_call_instr_node(call_expr_node_t * c)
{
    c_assert(c);

    instr_node_t * r = make_instr_node(INT_CALL);
    r->node.call->c = c;

    return r;
}

instr_node_t * make_return_instr_node(expr_node_t * e)
{
    c_assert(e);

    instr_node_t * r = make_instr_node(INT_RETURN);
    r->node.ret->expr = e;

    return r;
}

instr_node_t * make_super_instr_node(param_eff_expr_node_t * p)
{
    c_assert(p);

    instr_node_t * r = make_instr_node(INT_SUPER);
    r->node.super->params = p;

    return r;
}

instr_node_t * make_affect_instr_node(idf_t lv, rvalue_node_t * rv)
{
    c_assert(rv);

    instr_node_t * r = make_instr_node(INT_AFFECT);
    r->node.aff->lvalue = lv;
    r->node.aff->rvalue = rv;

    return r;
}


bloc_instr_node_t * make_bloc_instr_node()
{
    bloc_instr_node_t * r =
	(bloc_instr_node_t *)malloc(sizeof(bloc_instr_node_t));

    c_assert2(r, "malloc failed");

    c_warning2(0, "TODO TDS");
    r->tds = NULL;
    r->instrs = create_vector(8);

    return r;
}

void add_instr_bloc(bloc_instr_node_t * b, instr_node_t * i)
{
    c_assert(b);
    c_assert(i);

    vector_add_element(b->instrs, i);
}

size_t count_instr_bloc(bloc_instr_node_t * b)
{
    c_assert(b)
    return vector_size(b->instrs);
}

instr_node_t * get_bloc_instr(bloc_instr_node_t * b, size_t index)
{
    c_assert(b);
    return (instr_node_t *)vector_get_element_at(b->instrs, index);
}


void free_instr_node(instr_node_t * n)
{
}


void free_loop_instr_node(loop_instr_node_t * n)
{
}

void free_cond_instr_node(cond_instr_node_t * n)
{
}

void free_call_instr_node(call_instr_node_t * n)
{
}

void free_return_instr_node(return_instr_node_t * n)
{
}

void free_super_instr_node(super_instr_node_t * n)
{
}

void free_affect_instr_node(affect_instr_node_t* n)
{
}


void free_bloc_instr_node(bloc_instr_node_t * n)
{
}



