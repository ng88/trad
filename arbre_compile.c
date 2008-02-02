
#include <stdlib.h>

#include "arbre_compile.h"

#include "assert.h"
#include "lexique.h"

#include "error.h"

extern lexique_t * c_lexique;

/** Implementation de la compilation vers C */

#define CLASS_NAME_PREFIX "mc_class_"
#define FUNC_NAME_PREFIX  "mc_func_"
#define FIELD_NAME_PREFIX "mc_field_"
#define LVAR_NAME_PREFIX  "mc_lvar_"


compile_env_t * make_compile_env(char * dest)
{
    FILE * f = fopen(dest, "w");

    if(!f)
	return NULL;

    compile_env_t * c =
	(compile_env_t*)malloc(sizeof(compile_env_t));

    c_assert(c);

    c->dest = f;

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


    fputs("#include <stdio.h>\n", e->dest);
    fputs("#include <stdlib.h>\n\n", e->dest);


    compile_tds(e, b);


    fputs("int main()\n{\n", e->dest);
    fputs("   return EXIT_SUCCESS;\n}\n", e->dest);


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

    switch(t->otype)
    {
    case OBJ_CLASS:
	compile_class_node(e, t->infos.cl);
	break;
    case OBJ_PROC:
    case OBJ_FUNC:
    case OBJ_CTOR:
	compile_function_node(e, t->infos.fn);
	break;
    case OBJ_FIELD:
	/* pas compile ici mais dans compile_field()*/
	break;
    case OBJ_PARAM:
	/* traite das compile_function() */
	break;
    case OBJ_LOCAL_VAR:
	fputs("\t", e->dest);
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
	}
    }
    else
    {
	c_assert(t->type.uclass);
	fputs("struct " CLASS_NAME_PREFIX, e->dest);
	fputs(lexique_get(c_lexique, t->type.uclass->name_index)
	      , e->dest);
	fputs(" *", e->dest);
    }
}

void compile_field(compile_env_t * e, class_node_t * cl)
{
    c_assert(e && cl);

    if(cl->super)
	compile_field(e, cl->super);

    size_t s = tds_count(cl->tds);
    size_t i;

    for(i = 0; i < s; ++i)
    {
	 tds_entry_t * t = tds_get_entry(cl->tds, i);

	 if(t->otype == OBJ_FIELD)
	 {
	     fputs("\t", e->dest);
	     compile_var_type(e, t->type);

	     fputs(" " FIELD_NAME_PREFIX, e->dest);
	     fputs(lexique_get(c_lexique, cl->name_index), e->dest);
	     fputs("_", e->dest);
	     fputs(lexique_get(c_lexique, t->name_index), e->dest);

	     fputs(";\n", e->dest);

	 }
    }
}

void compile_class_node(compile_env_t * e, class_node_t * cl)
{
    c_assert(e && cl);

    fprintf(e->dest, "struct " CLASS_NAME_PREFIX  "%s\n{\n", 
	    lexique_get(c_lexique, cl->name_index));

    compile_field(e, cl);

    fputs("\n};\n", e->dest);

    compile_tds(e, cl->tds);

}

void compile_function_node(compile_env_t * e, function_node_t * fn)
{
    c_assert(e && fn);
    c_assert(fn->parent);

    compile_var_type(e, fn->ret_type);
    
    fputs(" " FUNC_NAME_PREFIX, e->dest);
    fputs(lexique_get(c_lexique, fn->parent->name_index), e->dest);

    if(fn->name_index != CTOR_NAME)
    {
	fputs("_", e->dest);
	fputs(lexique_get(c_lexique, fn->name_index), e->dest);
    }

    fputs("(", e->dest);

    compile_type_list(e, fn);

    fputs(")\n", e->dest);

    compile_bloc_instr_node(e, fn->block);

}




void compile_type_list(compile_env_t * e, function_node_t * fn)
{
    c_assert(fn && fn->params && fn->block && fn->block->tds && e);

    size_t n = vector_size(fn->params);
    size_t i;

    if(!n)
	return;

    /* index de debut des parametres */
    size_t param_start = tds_count(fn->block->tds) - n;

    for(i = 0; i < n; ++i)
    {
	if(i)
	    fputs(", ", e->dest);

	param_dec_t * p =
	    (param_dec_t *)vector_get_element_at(fn->params, i);

	compile_var_type(e, p->type);

	fputs(" " LVAR_NAME_PREFIX, e->dest);

	fputs(lexique_get(c_lexique,
			  tds_get_entry(fn->block->tds, param_start + i)->name_index
		         )
	      , e->dest);

    }
}


void compile_bloc_instr_node(compile_env_t * e, bloc_instr_node_t * n)
{
    c_assert(n);

    fputs("{\n", e->dest);

    if(n->tds)
        compile_tds(e, n->tds);

    int i;
    int s = count_instr_bloc(n);
    for(i = 0; i < s; ++i)
	;//compile_instr_node(e->dest, get_bloc_instr(n, i));

    fputs("}\n", e->dest);
}

