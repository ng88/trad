
#include <stdlib.h>

#include "assert.h"
#include "arbre_expr.h"
#include "lexique.h"

extern lexique_t * c_lexique;

expr_node_t * make_expr_node(expr_node_type_t t)
{
    expr_node_t * r = malloc(sizeof(expr_node_t));
    c_assert2(r, "malloc failed");

    r->type = t;

    return r;
}


expr_node_t * make_binary_expr_node(bin_expr_node_type_t t, expr_node_t * g, expr_node_t * d)
{
    c_assert(g && d);

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
    c_assert(f);

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

expr_node_t * make_constant_str_expr_node(size_t index_str)
{
    expr_node_t * r = make_constant_expr_node(CNT_STR);
    r->node.cst->val.index_str = index_str;
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

expr_node_t * make_constant_dbl_expr_node(double vd)
{
    expr_node_t * r = make_constant_expr_node(CNT_DBL);
    r->node.cst->val.vdouble = vd;
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
