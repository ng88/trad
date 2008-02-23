
#include <stdlib.h>
#include <string.h>

#include "arbre_compile.h"
#include "arbre_printer.h"
#include "tds.h"

#include "assert.h"
#include "lexique.h"
#include "anasyn.h"
#include "error.h"

extern lexique_t * c_lexique;

/** Implementation de la compilation vers C */

#define CLASS_NAME_PREFIX "mc_class_"
#define CLFNS_NAME_PREFIX "fns_class_"
#define FUNC_NAME_PREFIX  "mc_func_"
#define CTOR_NAME_PREFIX  "mc_ctor_"
#define NEW_NAME_PREFIX  "mc_new_"
#define FIELD_NAME_PREFIX "mc_field_"
#define LVAR_NAME_PREFIX  "mc_lvar_"
#define EXPORT_PREFIX    "c_export_"

#define INDENT(e) (print_indent((e)->dest, (e)->indent))


char * get_C_name(bool is_struct, char * prefix, idf_t name1, idf_t name2, name_t_t n)
{
    /* taille = longuer max idf + taille("struct ")
     + max(taille(prefix)) + un peu de marge */
    enum { B_SIZE = MAX_IDF_LEN + 64 };
    static char buff[B_SIZE];

    char * sn;

    switch(n)
    {
    case NTT_PTR: sn = " * "; break;
    case NTT_STRUCT: sn = "\n{\n"; break;
    case NTT_NONE: sn = ""; break;
    case NTT_FIELD: sn = ";\n"; break;
    case NTT_FUNC: sn = "_"; break;
    }

    if(name2)
	snprintf(buff, B_SIZE, "%s%s%s_%s%s",
		 is_struct ? "struct " : "",
		 prefix,
		 get_idf(name1),
		 get_idf(name2),
		 sn
	    );
    else    
	snprintf(buff, B_SIZE, "%s%s%s%s",
		 is_struct ? "struct " : "",
		 prefix,
		 get_idf(name1),
		 sn
	    );

    return buff;
}

compile_env_t * make_compile_env(char * dest)
{
    FILE * f = fopen(dest, "w");

    if(!f)
	return NULL;

    compile_env_t * c =
	(compile_env_t*)malloc(sizeof(compile_env_t));

    c_assert(c);

    c->dest = f;
    c->indent = 0;

    return c;
}

void free_compile_env(compile_env_t * e)
{
    c_assert(e);

    fclose(e->dest);

    free(e);
}

void compile_start(compile_env_t * e, tree_base_t * b, function_node_t * entry_point)
{
    c_assert(e && b);

    if(!entry_point)
	raise_error(ET_MAIN_NOT_FOUND);

    function_node_t * dctor = get_default_ctor(entry_point->parent);

    if(!dctor)
	raise_error(ET_MAIN_WITHOUT_DCTOR);

    fputs("#include <stdio.h>\n"
          "#include <string.h>\n"
          "#include <stdlib.h>\n\n"
	  "/* Fonctions diverses/builtins */\n\n"
	  "void p_failed(const char * err)\n{\n"
	  "\tfprintf(stderr, \"execution failed: %s\\n\", err);\n"
	  "\tabort();\n"
	  "}\n\n"
	  "enum { GETC_BUFF_SIZE = 1024, GETI_BUFF_SIZE = 256 };\n\n"
	  "#define c_export_freestr(s) free((s))\n"
	  "#define c_export_freeobj(o) free((o))\n"
	  "#define c_export_prints(s) fputs((s), stdout)\n"
	  "#define c_export_printi(i) printf(\"%d\", (i))\n"
	  "#define c_export_isnil(v) ((v) == c_export_nil)\n"
	  "#define c_export_nil NULL\n\n"
	  "char * c_export_getchar()\n"
	  "{\n"
	  "\tchar buff[GETC_BUFF_SIZE];\n"
	  "\tfgets(buff, GETC_BUFF_SIZE, stdin);\n"
	  "\tbuff[GETC_BUFF_SIZE - 1] = '\\0';\n"
	  "\treturn strdup(buff);\n"
	  "}\n\n"
	  "int c_export_getint()\n"
	  "{\n"
	  "\tchar buff[GETI_BUFF_SIZE];\n"
	  "\tfgets(buff, GETI_BUFF_SIZE, stdin);\n"
	  "\tbuff[GETI_BUFF_SIZE - 1] = '\\0';\n"
	  "\treturn atoi(buff);\n"
	  "}\n\n"
	  "char * c_export_substring(char * src, int first, int n)\n"
	  "{\n"
	  "\tint s = strlen(src);\n"
	  "\tif(first >= s)\n"
	  "\t\treturn strdup(\"\");\n"
	  "\n"
	  "\tif(first + n >= s)\n"
	  "\t\tn = s - first - 1;\n"
	  "\n"
	  "\tchar * ret = (char*)malloc(n + 1);\n"
	  "\tmemcpy(ret, src + first, n + 1);\n"
	  "\tret[n] = '\\0';\n"
	  "\n"
	  "\treturn ret;\n"
	  "}\n"
	  , e->dest);

    fputs("/* class declaration */\n", e->dest);
    e->header_mode = true;
    e->class_header = true;
    compile_tds(e, b);

    fputs("\n\n/* function declaration */\n", e->dest);
    e->class_header = false;
    compile_tds(e, b);

    fputs("\n\n/* code */\n", e->dest);
    e->header_mode = false;
    compile_tds(e, b);

    fputs("int main()\n{\n\t", e->dest);
    compile_function_name(e, entry_point, FUNC_NAME_PREFIX);
    fputs("(", e->dest);
    compile_function_name(e, dctor, NEW_NAME_PREFIX);
    fputs("());\n\treturn EXIT_SUCCESS;\n}\n", e->dest);


}


void compile_tds(compile_env_t * e, tds_t * t)
{
    c_assert(e && t);
    size_t s = tds_count(t);
    size_t i;

    for(i = 0; i < s; ++i)
	compile_tds_entry(e, tds_get_entry(t, i));

}

void compile_tds_entry(compile_env_t * e, tds_entry_t * t)
{
    c_assert(e && t);

    if(t->cexport)
	return;

    switch(t->otype)
    {
    case OBJ_CLASS:
	compile_class_node(e, t->infos.cl);
	break;
    case OBJ_CTOR:
	compile_constructor_node(e, t->infos.fn);
	break;	
    case OBJ_PROC:
    case OBJ_FUNC:
	compile_function_node(e, t->infos.fn);
	break;
    case OBJ_FIELD:
	/* pas compile ici mais dans compile_field()*/
	break;
    case OBJ_PARAM:
	/* traite das compile_function() */
	break;
    case OBJ_LOCAL_VAR:
	INDENT(e);
	compile_var_type(e, t->type);
	fputs(" " LVAR_NAME_PREFIX, e->dest);
	fputs(lexique_get(c_lexique, t->name_index), e->dest);

	/*if(!t->type->type_prim)
	  fputs("\n", e->dest);*/

	fputs(" = 0;\n", e->dest);
	break;
    }

}

void compile_var_type(compile_env_t * e, var_type_t * t)
{
    c_assert(e);

    if(!t)
	fputs("void", e->dest);
    else if(t->type_prim)
    {
	switch(t->type.prim)
	{
	case PT_STRING: fputs("char *", e->dest); break;
	case PT_INT: fputs("int", e->dest); break;
	case PT_REAL: fputs("double", e->dest); break;
	case PT_UNKNOW: c_assert2(false, "unknown type"); break;
	case PT_NIL:
	case PT_ANY: fputs("void *", e->dest); break;
	}
    }
    else
    {
	c_assert(t->type.uclass);
	fputs(get_C_name(true, CLASS_NAME_PREFIX, t->type.uclass->name_index, 0, NTT_PTR), e->dest);
    }
    fputs(" ", e->dest);
}

void compile_fields(compile_env_t * e, class_node_t * cl)
{
    c_assert(e && cl);

    if(cl->super)
	compile_fields(e, cl->super);

    size_t s = tds_count(cl->tds);
    size_t i;

    for(i = 0; i < s; ++i)
    {
	 tds_entry_t * t = tds_get_entry(cl->tds, i);

	 if(t->otype == OBJ_FIELD)
	 {
	     fputs("\t", e->dest);
	     compile_var_type(e, t->type);
	     fputs(get_C_name(false, FIELD_NAME_PREFIX, cl->name_index, t->name_index, NTT_FIELD), e->dest);
	 }
    }
}

void compile_functions(compile_env_t * e, class_node_t * cl,  class_node_t * last, bool dec_mode)
{
    c_assert(e && cl);

    if(cl->super)
	compile_functions(e, cl->super, last, dec_mode);

    size_t s = tds_count(cl->tds);
    size_t i;

    for(i = 0; i < s; ++i)
    {
	 tds_entry_t * t = tds_get_entry(cl->tds, i);

	 if(t->otype == OBJ_FUNC || t->otype == OBJ_PROC)
	 {
	     if(!is_an_overloaded_function(t->infos.fn))
	     {
		 if(dec_mode)
		 {
		     fputs("\t", e->dest);
		     compile_function_type(e, get_last_overload(t->infos.fn, last));
		     fputs(";\n", e->dest);
		 }
		 else
		 {
		     fputs("\t\tfns.", e->dest);
		     compile_function_name(e, t->infos.fn, NULL);
		     fputs(" = &", e->dest);
		     compile_function_name(e, get_last_overload(t->infos.fn, last), FUNC_NAME_PREFIX);
		     fputs(";\n", e->dest);
		 }
	     }
	 }
    }
}

void compile_function_type(compile_env_t * e, function_node_t * fn)
{
    c_assert(e && fn && fn->params);

    compile_var_type(e, fn->ret_type);

    fputs("(*", e->dest);
    compile_function_name(e, fn, NULL);
    fputs(")(", e->dest);
    compile_type_list(e, fn, true, true);
    fputs(")", e->dest);
}

void compile_function_name(compile_env_t * e, function_node_t * fn, char * prefix)
{
    c_assert(e && fn && fn->params);

    if(prefix)
	fputs(get_C_name(false, prefix, fn->parent->name_index, fn->name_index, NTT_NONE), e->dest);
    else
	fputs(get_C_name(false, FUNC_NAME_PREFIX, fn->name_index, 0, NTT_NONE), e->dest);

    size_t s = vector_size(fn->params);
    size_t i;

    for(i = 0; i < s; ++i)
    {
	param_dec_t * p =
	    (param_dec_t *)vector_get_element_at(fn->params, i);

	fputs("_", e->dest);
	fputs(get_var_type(p->type), e->dest);
    }

}


void compile_class_node(compile_env_t * e, class_node_t * cl)
{
    c_assert(e && cl);

    if(e->header_mode)
    {
	if(e->class_header)
	    fputs(get_C_name(true, CLASS_NAME_PREFIX, cl->name_index, 0, NTT_FIELD), e->dest);
	else
	    compile_tds(e, cl->tds);
    }
    else
    {
	fputs(get_C_name(true, CLFNS_NAME_PREFIX, cl->name_index, 0, NTT_STRUCT), e->dest);

	compile_functions(e, cl, cl, true);

	fputs("\n};\n", e->dest);

	char * fnsn = get_C_name(true, CLFNS_NAME_PREFIX, cl->name_index, 0, NTT_NONE);

	fprintf(e->dest,
		"%s * get_fns_for_%s()\n"
		"{\n"
		"\tstatic %s fns;\n"
		"\tstatic int first = 1;\n"
		"\tif(first)\n\t{\n"
		"\t\tfirst = 0;\n",
		fnsn, lexique_get(c_lexique, cl->name_index), fnsn
	    );

	compile_functions(e, cl, cl, false);


	fprintf(e->dest,
		"\t}\n"
		"\treturn &fns;\n}\n"
	    );

	fputs(get_C_name(true, CLASS_NAME_PREFIX, cl->name_index, 0, NTT_STRUCT), e->dest);

	fprintf(e->dest, "\t%sfns;\n", 
		get_C_name(true, CLFNS_NAME_PREFIX, cl->name_index, 0, NTT_PTR)
	    );

	compile_fields(e, cl);

	fputs("\n};\n", e->dest);

	compile_tds(e, cl->tds);
    }

}

void compile_function_node(compile_env_t * e, function_node_t * fn)
{
    c_assert(e && fn);
    c_assert(fn->parent);

    compile_var_type(e, fn->ret_type);

    compile_function_name(e, fn, 
	   fn->name_index == CTOR_NAME ? CTOR_NAME_PREFIX : FUNC_NAME_PREFIX);

    fputs("(", e->dest);

    compile_type_list(e, fn, true, true);

    fputs(")", e->dest);


    if(e->header_mode)
	fputs(";\n", e->dest);
    else
    {
	fputs("\n", e->dest);
	compile_bloc_instr_node(e, fn->block);
    }

}

void compile_constructor_node(compile_env_t * e, function_node_t * fn)
{
    c_assert(e && fn);
    c_assert(fn->parent);
    c_assert(!fn->ret_type && fn->name_index == CTOR_NAME);


    char * retname = strdup(get_C_name(true, CLASS_NAME_PREFIX, fn->parent->name_index, 0, NTT_NONE));

    fputs(retname, e->dest);
    fputs(" * ", e->dest);

    compile_function_name(e, fn, NEW_NAME_PREFIX);

    fputs("(", e->dest);

    compile_type_list(e, fn, true, false);

    fputs(")", e->dest);

    if(e->header_mode)
	fputs(";\n", e->dest);
    else
    {


	fprintf(e->dest,
		"\n{\n\t%s * this = (%s*)malloc(sizeof(*this));\n"
		"\tif(!this) p_failed(\"not enough heap memory!\");\n"
		"\tthis->fns = get_fns_for_%s();\n\t",
		retname, retname,
		lexique_get(c_lexique, fn->parent->name_index));

	compile_function_name(e, fn, CTOR_NAME_PREFIX);

	fputs("(", e->dest);

	compile_type_list(e, fn, false, true);

	fputs(
	    ");\n"
	    "\treturn this;\n"
	    "\n}\n"
	      , e->dest);
    }

    free(retname);
    compile_function_node(e, fn);
}




void compile_type_list(compile_env_t * e, function_node_t * fn, bool full, bool pthis)
{
    c_assert(fn && fn->params && fn->block && fn->block->tds && e);

    if(pthis)
    {
	if(full)
	{
	    var_type_t t;
	    t.type_prim = false;
	    t.type.uclass = fn->parent;
	    compile_var_type(e, &t);
	}
	
	fputs("this", e->dest);
    }

    size_t n = vector_size(fn->params);
    size_t i;

    if(!n)
	return;

    /* index de debut des parametres */
    size_t param_start = tds_count(fn->block->tds) - n;

    for(i = 0; i < n; ++i)
    {
	if(pthis || i)
	    fputs(", ", e->dest);

	param_dec_t * p =
	    (param_dec_t *)vector_get_element_at(fn->params, i);

	if(full)
	    compile_var_type(e, p->type);

	fputs(LVAR_NAME_PREFIX, e->dest);

	fputs(lexique_get(c_lexique,
			  tds_get_entry(fn->block->tds, param_start + i)->name_index
		         )
	      , e->dest);

    }
}


void compile_bloc_instr_node(compile_env_t * e, bloc_instr_node_t * n)
{
    c_assert(n && e);

    INDENT(e);
    fputs("{\n", e->dest);
    e->indent++;

    if(n->tds)
        compile_tds(e, n->tds);

    int i;
    int s = count_instr_bloc(n);
    for(i = 0; i < s; ++i)
	compile_instr_node(e, get_bloc_instr(n, i));

    e->indent--;
    INDENT(e);
    fputs("}\n", e->dest);
}

void compile_instr_node(compile_env_t * e, instr_node_t * n)
{
    c_assert(n && e);

    INDENT(e);

    switch(n->type)
    {
    case INT_LOOP: compile_loop_instr_node(e, n->node.loop); break;
    case INT_CALL: compile_call_instr_node(e, n->node.call); break;
    case INT_COND: compile_cond_instr_node(e, n->node.cond); break;
    case INT_RETURN: compile_return_instr_node(e, n->node.ret); break;
    case INT_SUPER: compile_super_instr_node(e, n->node.super); break;
    case INT_AFFECT: compile_affect_instr_node(e, n->node.aff); break;
    }

    fputc('\n', e->dest);
}

void compile_loop_instr_node(compile_env_t * e, loop_instr_node_t * n)
{
   c_assert(n && e);
    fputs("while(", e->dest);
    compile_expr_node(e, n->cond);
    fputs(")\n", e->dest);
    compile_bloc_instr_node(e, n->body);
}

void compile_call_instr_node(compile_env_t * e, call_instr_node_t * n)
{
    c_assert(n);
    compile_call_expr_node(e, n->c);
    fputc(';', e->dest);
}

void compile_cond_instr_node(compile_env_t * e, cond_instr_node_t * n)
{
    c_assert(n && e);
    fputs("if(", e->dest);
    compile_expr_node(e, n->cond);
    fputs(")\n", e->dest);
    compile_bloc_instr_node(e, n->btrue);
    if(n->bfalse)
    {
	INDENT(e);
	fputs("else\n", e->dest);
	compile_bloc_instr_node(e, n->bfalse);
    }
    INDENT(e);
}

void compile_return_instr_node(compile_env_t * e, return_instr_node_t * n)
{
    c_assert(n);
    fputs("return ", e->dest);
    compile_expr_node(e, n->expr);
    fputc(';', e->dest);
}

void compile_super_instr_node(compile_env_t * e, super_instr_node_t * n)
{

}

void compile_affect_instr_node(compile_env_t * e, affect_instr_node_t* n)
{
    c_assert(n);
    c_assert(n->lvalue_resolved);

    compile_var_idf(e, n->lvalue_resolved, true);

    fputs(" = ", e->dest);
    compile_rvalue_node(e, n->rvalue);
    fputc(';', e->dest);
}

void compile_expr_node(compile_env_t * e, expr_node_t * n)
{
    c_assert(n);

    switch(n->type)
    {
    case NT_BINARY:
	compile_binary_expr_node(e, n->node.bin);
	break;
    case NT_UNARY:
	compile_unary_expr_node(e, n->node.una);
	break;
    case NT_CONST:
	compile_constant_expr_node(e, n->node.cst);
	break;
    case NT_CALL:
	compile_call_expr_node(e, n->node.call);
	break;
    }
}

void compile_binary_expr_node(compile_env_t * e, bin_expr_node_t * n)
{
    c_assert(n && e);

    fputc('(',  e->dest);
    compile_expr_node(e, n->gauche);
    fputc(' ',  e->dest);

    fputs(get_bin_operator(n->type),  e->dest);

    fputc(' ',  e->dest);
    compile_expr_node(e, n->droit);
    fputc(')',  e->dest);
}

void compile_unary_expr_node(compile_env_t * e, una_expr_node_t * n)
{
    c_assert(n);

    fputc('(', e->dest);

    fputs(get_una_operator(n->type),  e->dest);

    fputc(' ',  e->dest);
    compile_expr_node(e, n->fils);
    fputc(')',  e->dest);
}

void compile_constant_expr_node(compile_env_t * e, cst_expr_node_t * n)
{
    c_assert(n && e);

    switch(n->type)
    {
    case CNT_INT:
	fprintf(e->dest, "%d", n->val.vint);
	break;
    case CNT_DBL:
	fprintf(e->dest, "((double)%g)", n->val.vdouble);
	break;
    case CNT_STR:
	fprintf(e->dest, "\"%s\"", lexique_get(c_lexique, n->val.index_str));
	break;
    }
}

void compile_new_expr_node(compile_env_t * e, new_expr_node_t * n)
{
}

void compile_rvalue_node(compile_env_t * e, rvalue_node_t * n)
{
    c_assert(n && e);

    switch(n->type)
    {
    case RNT_NEW:
	compile_new_expr_node(e, n->node.nnew);
	break;
    case RNT_EXPR:
	compile_expr_node(e, n->node.expr);
	break;
    }
}


void compile_call_expr_node(compile_env_t * e, call_expr_node_t * n)
{
    c_assert(n && e);

    switch(n->type)
    {
    case CENT_DIRECT:
	compile_direct_call_expr_node(e, n->node.dc);
	break;
    case CENT_MEMBER:
	compile_member_expr_node(e, n->node.mem);
	break;
    }
}


void compile_member_fn_call_expr_node(compile_env_t * e, member_expr_node_t * n)
{
    c_assert(n && e && n->f->type == DCENT_FN);
    c_assert(n->f && n->f->resolved && (n->f->resolved->otype & OBJ_FNP));

/*
  produit un truc du genre de :
  ({ struct mc_class_MaClasse * _this = (expression); (*(_this->fns->mc_func_ajout_integer_integer))(_this , parametres); });
*/

    function_node_t * fn = n->f->resolved->infos.fn;

    if(n->p)
    {
	fprintf(e->dest, "({%s _this = (", 
		get_C_name(true, CLASS_NAME_PREFIX, fn->parent->name_index, 0, NTT_PTR)
	    );

	compile_call_expr_node(e, n->p);

	fputs("); (*(_this->fns->", e->dest);

	compile_function_name(e, fn, NULL);

	fputs("))(_this", e->dest);
	compile_param_eff_expr_node(e, n->f->node.fnc->params, true);
	fputs("); })", e->dest);
    }
    else /* petite simplification */
    {

	fputs("(*(this->fns->", e->dest);

	compile_function_name(e, fn, NULL);

	fputs("))(this", e->dest);
	compile_param_eff_expr_node(e, n->f->node.fnc->params, true);
	fputs(")", e->dest);
    }
}

void compile_param_eff_expr_node(compile_env_t * e, param_eff_expr_node_t * n, bool comma)
{
    c_assert(e && n && n->params);

    int i;
    int s = param_eff_count(n);
    for(i = 0; i < s; ++i)
    {
	if(comma)
	    fputs(", ", e->dest);
	else
	    comma = true;

	compile_expr_node(e, param_eff_get(n, i));
    }

}


void compile_var_idf(compile_env_t * e, tds_entry_t * t, bool need_this)
{
    c_assert(e && t);

    bool pthis = need_this && (t->otype & OBJ_NO_LOCAL);

    if(pthis)
	fputs("(this->", e->dest);

    switch(t->otype)
    {
    case OBJ_CLASS:
    case OBJ_CTOR:
    case OBJ_PROC:
    case OBJ_FUNC:
	/* pas gere ici */
	break;
    case OBJ_FIELD:
	fputs(get_C_name(false, FIELD_NAME_PREFIX, t->infos.fl->parent->name_index, t->name_index, NTT_NONE), e->dest);
	break;
    case OBJ_PARAM:
    case OBJ_LOCAL_VAR:
	fputs(get_C_name(false, t->cexport ? EXPORT_PREFIX : LVAR_NAME_PREFIX, t->name_index, 0, NTT_NONE), e->dest);
	break;
    }

    if(pthis)
	fputs(")", e->dest);

}

void compile_direct_call_expr_node(compile_env_t * e, direct_call_expr_node_t * n)
{
    c_assert(n && e);

    c_assert(n->resolved);

    switch(n->type)
    {
    case DCENT_FN:
     {
	 if(n->resolved->cexport)
	 {
	     /* cas particulier des fonctions builtin */
	     fputs(get_C_name(false, EXPORT_PREFIX, n->resolved->infos.fn->name_index, 0, NTT_NONE), e->dest);
	     fputc('(', e->dest);
	     compile_param_eff_expr_node(e, n->node.fnc->params, false);
	     fputc(')', e->dest);
	 }
	 else
	 {
	     /* ici on a un appel direct du type fn(...)
		on va le transformer en this.fn(...) */

	     member_expr_node_t ct;
	     ct.p = NULL;
	     ct.f = n;

	     compile_member_fn_call_expr_node(e, &ct);
	 }
     }
     break;
    case DCENT_IDF:
	compile_var_idf(e, n->resolved, n->need_this);
	break;
    }
}

void compile_member_expr_node(compile_env_t * e, member_expr_node_t * n)
{
    c_assert(n && e);

    if(n->f->type == DCENT_IDF)
    {
	fputc('(', e->dest);
	compile_call_expr_node(e, n->p);
	fputs("->", e->dest);
	compile_direct_call_expr_node(e, n->f);
	fputc(')', e->dest);
    }
    else
	compile_member_fn_call_expr_node(e, n);
}


