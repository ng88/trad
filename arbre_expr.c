
#include <stdlib.h>

#include "assert.h"
#include "arbre_expr.h"
#include "lexique.h"

extern lexique_t * c_lexique;

expr_node_t * make_expr_node(expr_node_type_t t)
{
    expr_node_t * r = (expr_node_t *)malloc(sizeof(expr_node_t));
    c_assert2(r, "malloc failed");

    r->type = t;

    return r;
}


expr_node_t * make_binary_expr_node(bin_expr_node_type_t t, expr_node_t * g, expr_node_t * d)
{
    c_assert(g && d);

    expr_node_t * r = make_expr_node(NT_BINARY);

    r->node.bin = (bin_expr_node_t*)malloc(sizeof(bin_expr_node_t));
    c_assert2(r->node.bin, "malloc failed");

    r->node.bin->type = t;
    r->node.bin->gauche = g;
    r->node.bin->droit = d;

    return r ;
}

expr_node_t * make_unary_expr_node(una_expr_node_type_t t, expr_node_t * f)
{
    c_assert(f);

    expr_node_t * r = make_expr_node(NT_UNARY);

    r->node.una = (una_expr_node_t*)malloc(sizeof(una_expr_node_t));
    c_assert2(r->node.una, "malloc failed");

    r->node.una->type = t;
    r->node.una->fils = f;

    return r ;
}

expr_node_t * make_constant_expr_node(cst_expr_node_type_t t)
{
    expr_node_t * r = make_expr_node(NT_CONST);

    r->node.cst = (cst_expr_node_t*)malloc(sizeof(cst_expr_node_t));
    c_assert2(r->node.cst, "malloc failed");

    r->node.cst->type = t;

    return r ;
}

expr_node_t * make_constant_str_expr_node(size_t index_str)
{
    expr_node_t * r = make_constant_expr_node(CNT_STR);
    r->node.cst->val.index_str = index_str;
    return r;
}

expr_node_t * make_constant_idf_expr_node(idf_t vidf)
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

expr_node_t * make_constant_dbl_expr_node(double vd)
{
    expr_node_t * r = make_constant_expr_node(CNT_DBL);
    r->node.cst->val.vdouble = vd;
    return r;
}

param_eff_expr_node_t * make_param_eff_expr_node()
{
    param_eff_expr_node_t * r =
	(param_eff_expr_node_t *)malloc(sizeof(param_eff_expr_node_t));

    c_assert2(r, "malloc failed");

    r->params = create_vector(4);

    return r;
}

void add_param_eff(param_eff_expr_node_t * p, expr_node_t * e)
{
    c_assert(p);
    c_assert(e);

    vector_add_element(p->params, e);
}

size_t param_eff_count(param_eff_expr_node_t * p)
{
    c_assert(p);
    return vector_size(p->params);
}

expr_node_t * param_eff_get(param_eff_expr_node_t * p, size_t index)
{
    c_assert(p);
    return (expr_node_t *)vector_get_element_at(p->params, index);
}


new_expr_node_t *  make_new_expr_node(idf_t idf, param_eff_expr_node_t * p)
{
    new_expr_node_t * r =
	(new_expr_node_t *)malloc(sizeof(new_expr_node_t));

    c_assert2(r, "malloc failed");

    r->idf = idf;
    r->params = p;

    return r;
}

rvalue_node_t * make_rvalue_node(rvalue_node_type_t t)
{
    rvalue_node_t * r =
	(rvalue_node_t*)malloc(sizeof(rvalue_node_t));

    c_assert2(r, "malloc failed");

    r->type = t;

    return r;
}

rvalue_node_t * make_rvalue_expr_node(rvalue_node_type_t t, expr_node_t * n)
{
    rvalue_node_t * r = make_rvalue_node(RNT_EXPR);

    r->node.expr = n;

    return r;
}

rvalue_node_t * make_rvalue_new_node(rvalue_node_type_t t, new_expr_node_t * n)
{
    rvalue_node_t * r = make_rvalue_node(RNT_NEW);

    r->node.nnew = n;

    return r;
}




void print_expr_node(expr_node_t * n, FILE * f)
{
    c_assert(n);

    switch(n->type)
    {
    case NT_BINARY:
	print_binary_expr_node(n->node.bin, f);
	break;
    case NT_UNARY:
	print_unary_expr_node(n->node.una, f);
	break;
    case NT_CONST:
	print_constant_expr_node(n->node.cst, f);
	break;
    case NT_CALL:
	//TODO
	c_warning2(0, "TODO");
	break;
    }
}

void print_binary_expr_node(bin_expr_node_t * n, FILE * f)
{
    c_assert(n);

    fputc('(', f);
    print_expr_node(n->gauche, f);
    fputc(' ', f);

    switch(n->type)
    {
    case BNT_MUL:    fputc('*', f); break;
    case BNT_DIV:    fputc('/', f); break;
    case BNT_PLUS:   fputc('+', f); break;
    case BNT_MINUS:  fputc('-', f); break;
    case BNT_EQ:     fputc('=', f); break;
    case BNT_NE:     fputs("!=", f); break;
    case BNT_LE:     fputs("<=", f); break;
    case BNT_GE:     fputs(">=", f); break;
    case BNT_LT:     fputc('<', f); break;
    case BNT_GT:     fputc('>', f); break;
    case BNT_AND:    fputc('&', f); break;
    case BNT_OR:     fputs("||", f); break;
    }

    fputc(' ', f);
    print_expr_node(n->droit, f);
    fputc(')', f);
}

void print_unary_expr_node(una_expr_node_t * n, FILE * f)
{
    c_assert(n);

    fputc('(', f);

    switch(n->type)
    {
    case UNT_MINUS:
	fputc('-', f);
	break;
    }

    fputc(' ', f);
    print_expr_node(n->fils, f);
    fputc(')', f);
}

void print_constant_expr_node(cst_expr_node_t * n, FILE * f)
{

    c_assert(n);

    switch(n->type)
    {
    case CNT_INT:
	fprintf(f, "%d", n->val.vint);
	break;
    case CNT_DBL:
	fprintf(f, "%g", n->val.vdouble);
	break;
    case CNT_STR:
	fprintf(f, "\"%s\"", lexique_get(c_lexique, n->val.index_str));
	break;
    case CNT_IDF:
	fprintf(f, "IDF");
	break;
    }
}



void free_expr_node(expr_node_t * n)
{
    c_assert(n);

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
    case NT_CALL:
	//TODO
	c_warning2(0, "TODO");
	break;
    }
    free(n);
}


void free_binary_expr_node(bin_expr_node_t * n)
{
    c_assert(n);

    free_expr_node(n->gauche);
    free_expr_node(n->droit);
    free(n);
}

void free_unary_expr_node(una_expr_node_t * n)
{
    c_assert(n);

    free_expr_node(n->fils);
    free(n);
}

void free_constant_expr_node(cst_expr_node_t * n)
{
    c_assert(n);

    free(n);
}
