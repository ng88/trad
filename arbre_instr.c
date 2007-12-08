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

    r->node.loop = (loop_instr_node_t*)malloc(sizeof(loop_instr_node_t));
    c_assert2(r->node.loop, "malloc failed");

    r->node.loop->cond = c;
    r->node.loop->body = b;

    return r;
}

instr_node_t * make_cond_instr_node(expr_node_t * c, bloc_instr_node_t * bt,
				    bloc_instr_node_t * bf)
{
    c_assert(c && bt);

    instr_node_t * r = make_instr_node(INT_COND);

    r->node.cond = (cond_instr_node_t*)malloc(sizeof(cond_instr_node_t));
    c_assert2(r->node.cond, "malloc failed");

    r->node.cond->cond = c;
    r->node.cond->btrue = bt;
    r->node.cond->bfalse = bf;

    return r;
}

instr_node_t * make_call_instr_node(call_expr_node_t * c)
{
    c_assert(c);

    instr_node_t * r = make_instr_node(INT_CALL);

    r->node.call = (call_instr_node_t*)malloc(sizeof(call_instr_node_t));
    c_assert2(r->node.call, "malloc failed");

    r->node.call->c = c;

    return r;
}

instr_node_t * make_return_instr_node(expr_node_t * e)
{
    c_assert(e);

    instr_node_t * r = make_instr_node(INT_RETURN);

    r->node.ret = (return_instr_node_t*)malloc(sizeof(return_instr_node_t));
    c_assert2(r->node.ret, "malloc failed");

    r->node.ret->expr = e;

    return r;
}

instr_node_t * make_super_instr_node(param_eff_expr_node_t * p)
{
    c_assert(p);

    instr_node_t * r = make_instr_node(INT_SUPER);

    r->node.super = (super_instr_node_t*)malloc(sizeof(super_instr_node_t));
    c_assert2(r->node.super, "malloc failed");

    r->node.super->params = p;

    return r;
}

instr_node_t * make_affect_instr_node(idf_t lv, rvalue_node_t * rv)
{
    c_assert(rv);

    instr_node_t * r = make_instr_node(INT_AFFECT);

    r->node.aff = (affect_instr_node_t*)malloc(sizeof(affect_instr_node_t));
    c_assert2(r->node.aff, "malloc failed");

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




void print_instr_node(instr_node_t * n, FILE * f, int indent)
{
    c_assert(n);

    print_indent(f, indent);

    switch(n->type)
    {
    case INT_LOOP: print_loop_instr_node(n->node.loop, f, indent); break;
    case INT_CALL: print_call_instr_node(n->node.call, f, indent); break;
    case INT_COND: print_cond_instr_node(n->node.cond, f, indent); break;
    case INT_RETURN: print_return_instr_node(n->node.ret, f, indent); break;
    case INT_SUPER: print_super_instr_node(n->node.super, f, indent); break;
    case INT_AFFECT: print_affect_instr_node(n->node.aff, f, indent); break;
    }

    fputc('\n', f);
}

void print_loop_instr_node(loop_instr_node_t * n, FILE * f, int indent)
{
    c_assert(n);
    fputs("while ", f);
    print_expr_node(n->cond, f);
    fputs(" do\n", f);
    print_bloc_instr_node(n->body, f, indent);

}

void print_cond_instr_node(cond_instr_node_t * n, FILE * f, int indent)
{
    c_assert(n);
    fputs("if ", f);
    print_expr_node(n->cond, f);
    fputs(" then\n", f);
    print_bloc_instr_node(n->btrue, f, indent);
    if(n->bfalse)
    {
	fputc('\n', f);
	print_indent(f, indent);
	fputs("else\n", f);
	print_bloc_instr_node(n->bfalse, f, indent);
    }
    fputc('\n', f);
    print_indent(f, indent);
    fputs("endif", f);
}

void print_call_instr_node(call_instr_node_t * n, FILE * f, int indent)
{
    c_assert(n);
    print_call_expr_node(n->c, f);
    fputc(';', f);
}

void print_return_instr_node(return_instr_node_t * n, FILE * f, int indent)
{
    c_assert(n);
    fputs("return(", f);
    print_expr_node(n->expr, f);
    fputc(')', f);
    fputc(';', f);
}

void print_super_instr_node(super_instr_node_t * n, FILE * f, int indent)
{
    c_assert(n);
    fputs("super", f);
    print_param_eff_expr_node(n->params, f);
    fputc(';', f);
}

void print_affect_instr_node(affect_instr_node_t* n, FILE * f, int indent)
{
    c_assert(n);
    //TODO
    fputs("IDF := ", f);
    print_rvalue_node(n->rvalue, f);
    fputc(';', f);
}


void print_bloc_instr_node(bloc_instr_node_t * n, FILE * f, int indent)
{
    c_assert(n);

    print_indent(f, indent);
    fputs("{\n", f);

    indent++;

    int i;
    int s = count_instr_bloc(n);
    for(i = 0; i < s; ++i)
	print_instr_node(get_bloc_instr(n, i), f, indent);

    print_indent(f, indent - 1);
    fputs("}", f);
}

void print_indent(FILE * f, int indent)
{
    int i;
    for(i = 0; i < indent; ++i)
	fputc('\t', f);
}


void free_instr_node(instr_node_t * n)
{
    c_assert(n);
    switch(n->type)
    {
    case INT_LOOP: free_loop_instr_node(n->node.loop); break;
    case INT_CALL: free_call_instr_node(n->node.call); break;
    case INT_COND: free_cond_instr_node(n->node.cond); break;
    case INT_RETURN: free_return_instr_node(n->node.ret); break;
    case INT_SUPER: free_super_instr_node(n->node.super); break;
    case INT_AFFECT: free_affect_instr_node(n->node.aff); break;
    }
    free(n);
}


void free_loop_instr_node(loop_instr_node_t * n)
{
    c_assert(n);
    free_expr_node(n->cond);
    free_bloc_instr_node(n->body);
    free(n);
}

void free_cond_instr_node(cond_instr_node_t * n)
{
    c_assert(n);
    free_expr_node(n->cond);
    free_bloc_instr_node(n->btrue);
    if(n->bfalse) free_bloc_instr_node(n->bfalse);
    free(n);
}

void free_call_instr_node(call_instr_node_t * n)
{
    c_assert(n);
    free_call_expr_node(n->c);
    free(n);
}

void free_return_instr_node(return_instr_node_t * n)
{
    c_assert(n);
    free_expr_node(n->expr);
    free(n);
}

void free_super_instr_node(super_instr_node_t * n)
{
    c_assert(n);
    free_param_eff_expr_node(n->params);
    free(n);
}

void free_affect_instr_node(affect_instr_node_t* n)
{
    c_assert(n);
    free_rvalue_node(n->rvalue);
    free(n);
}


void free_bloc_instr_node(bloc_instr_node_t * n)
{
    c_assert(n);
    c_warning2(0, "TODO TDS");

    int i;
    int s = count_instr_bloc(n);
    for(i = 0; i < s; ++i)
	free_instr_node(get_bloc_instr(n, i));

    free_vector(n->instrs, 0);
    free(n);
}



