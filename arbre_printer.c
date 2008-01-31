
#include <stdlib.h>

#include "assert.h"
#include "arbre_printer.h"
#include "lexique.h"

extern lexique_t * c_lexique;



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
	fputs("IDF", f);
	break;
    }
}



void print_param_eff_expr_node(param_eff_expr_node_t * n, FILE * f)
{
    c_assert(n);

    fputc('(', f);
    int i;
    int s = param_eff_count(n);
    for(i = 0; i < s; ++i)
    {
	if(i)
	    fputs(", ", f);

	print_expr_node(param_eff_get(n, i), f);
    }

    fputc(')', f);
}

void print_new_expr_node(new_expr_node_t * n, FILE * f)
{
    c_assert(n);

    fputs("new IDF", f);

    print_param_eff_expr_node(n->params, f);
}

void print_rvalue_node(rvalue_node_t * n, FILE * f)
{
    c_assert(n);

    switch(n->type)
    {
    case RNT_NEW:
	print_new_expr_node(n->node.nnew, f);
	break;
    case RNT_EXPR:
	print_expr_node(n->node.expr, f);
	break;
    }
}

void print_call_expr_node(call_expr_node_t * n, FILE * f)
{
    c_assert(n);

    switch(n->type)
    {
    case CENT_DIRECT:
	print_direct_call_expr_node(n->node.dc, f);
	break;
    case CENT_MEMBER:
	print_member_expr_node(n->node.mem, f);
	break;
    }
}

void print_direct_call_expr_node(direct_call_expr_node_t * n, FILE * f)
{
    c_assert(n);

    switch(n->type)
    {
    case DCENT_FN:
	print_fn_call_expr_node(n->node.fnc, f);
	break;
    case DCENT_IDF:
	fputs("IDF", f);
	break;
    }
}

void print_member_expr_node(member_expr_node_t * n, FILE * f)
{
    c_assert(n);
    fputc('(', f);
    print_call_expr_node(n->p, f);
    fputc('.', f);
    print_direct_call_expr_node(n->f, f);
    fputc(')', f);
}

void print_fn_call_expr_node(fn_call_expr_node_t * n, FILE * f)
{
    c_assert(n);
    fputs("IDF", f);
    print_param_eff_expr_node(n->params, f);
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

    if(n->tds)
	print_tds(n->tds, f, indent);

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


void print_tds(tds_t * t, FILE * f, int indent)
{
    c_assert(t);
    size_t s = tds_count(t);
    size_t i;

    print_indent(f, indent);
    fputs("Local symbol table: \n", f);

    for(i = 0; i < s; ++i)
	print_tds_entry(tds_get_entry(t, i), f, indent);

}


void print_tds_entry(tds_entry_t * t, FILE * f, int indent)
{
    c_assert(t);

    switch(t->otype)
    {
    case OBJ_CLASS:
	print_class_node(t->infos.cl, f, indent);
	break;
    case OBJ_PROC:
    case OBJ_FUNC:
    case OBJ_CTOR:
	print_function_node(t->infos.fn, f, indent);
	break;
    case OBJ_FIELD:
	print_indent(f, indent);
	print_scope(t->infos.field_scope, f);
	fprintf(f, " field `%s', type=",
		lexique_get(c_lexique, t->name_index)
	    );
	print_var_type(t->type, f);
	fputs("\n", f);
	break;
    case OBJ_PARAM:
    case OBJ_LOCAL_VAR:
	print_indent(f, indent);
	fprintf(f, "Symbol `%s', type=",
		lexique_get(c_lexique, t->name_index)
	    );
	print_var_type(t->type, f);
	fputs(" (local var)\n", f);
	break;
    }

}

void print_var_type(var_type_t * t, FILE * f)
{
    fputs(get_var_type(t), f);
}












void print_class_node(class_node_t * cl, FILE * f, int indent)
{
    c_assert(cl);

    print_indent(f, indent);
    fprintf(f, "class %s inherits %s\n",
	    lexique_get(c_lexique, cl->name_index),
	    cl->super ? lexique_get(c_lexique, cl->super->name_index) : "<object>"
	    );

    print_indent(f, indent);
    fputs("begin\n", f);

    print_tds(cl->tds, f, indent + 1);

    print_indent(f, indent);
    fputs("end\n", f);
}

void print_function_node(function_node_t * fn, FILE * f, int indent)
{
    c_assert(fn);

    print_indent(f, indent);
    print_scope(fn->scope, f);


    if(fn->name_index == CTOR_NAME)
	fputs("constructor(", f);
    else if(fn->ret_type)
	fprintf(f, " function %s(", 
		lexique_get(c_lexique, fn->name_index));
    else
	fprintf(f, " procedure %s(", 
		lexique_get(c_lexique, fn->name_index));


    print_type_list(fn->params, f);

    fputs(")\n", f);

    print_bloc_instr_node(fn->block, f, indent + 1);

}

void print_type_list(vector_t * params, FILE * f)
{
    c_assert(params);

    size_t n = vector_size(params);
    size_t i;

    for(i = 0; i < n; ++i)
    {
	if(i)
	    fputs(", ", f);

	param_dec_t * p =
	    (param_dec_t *)vector_get_element_at(params, i);

	print_var_type(p->type, f);
	fprintf(f, " p%d", i);
    }
}


void print_scope(scope_t s, FILE * f)
{
    switch(s)
    {
    case ST_PRIVATE: fputs("private", f); break;
    case ST_PUBLIC: fputs("public", f); break;
    }
}














