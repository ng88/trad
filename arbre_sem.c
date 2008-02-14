
#include <stdlib.h>
#include <string.h>

#include "assert.h"
#include "arbre_sem.h"
#include "error.h"

#include "anasyn.h"
#include "arbre_printer.h"
#include "lexique.h"


extern lexique_t * c_lexique;

/** Matrice indiquant le type resultant d'une operation
 binaire entre 2 type primitifs */

static primitive_type_t BIN_TYPE_MATRIX[PT_UNKNOW + 1][PT_UNKNOW + 1] =
                       {
			   {PT_INT, PT_REAL, PT_UNKNOW},      /* PT_INT    */
			   {PT_REAL, PT_REAL, PT_UNKNOW},     /* PT_REAL   */
			   {PT_UNKNOW, PT_UNKNOW, PT_UNKNOW}, /* PT_UNKNOW */
                       };

void resolve_expr_node(expr_node_t * e, resolve_env_t * f)
{
    c_assert(e && f);

    switch(e->type)
    {
    case NT_BINARY:
	resolve_binary_expr_node(e->node.bin, f);
	break;
    case NT_UNARY:
	resolve_unary_expr_node(e->node.una, f);
	break;
    case NT_CONST:
	resolve_constant_expr_node(e->node.cst, f);
	break;
    case NT_CALL:
	TYPE_SET_UNKNOWN(&f->context);
	resolve_call_expr_node(e->node.call, f);
	break;
    }
}

void resolve_binary_expr_node(bin_expr_node_t * e, resolve_env_t * f)
{
    c_assert(e && f);

    var_type_t * gauche, * droit;
    bool err = true;

    TYPE_SET_UNKNOWN(&f->type);
    resolve_expr_node(e->gauche, f);
    gauche = &f->type;

    TYPE_SET_UNKNOWN(&f->type);
    resolve_expr_node(e->droit, f);
    droit = &f->type;

    /* ssi 2 types primitifs */
    if(droit->type_prim && gauche->type_prim)
    {
	/* ssi on a 2 scalaires */
	if(droit->type.prim != PT_STRING && 
	   gauche->type.prim != PT_STRING)
	{
	    err = false;

	    f->type.type_prim = true;
	    f->type.type.prim = BIN_TYPE_MATRIX[gauche->type.prim][droit->type.prim];
	}
    }

    if(err)
	raise_error(ET_TYPE_BIN_ERR, get_var_type(gauche),
		    get_bin_operator(e->type),
		    get_var_type(droit));

}

void resolve_unary_expr_node(una_expr_node_t * e, resolve_env_t * f)
{
    c_assert(e && f);

    var_type_t * fils;
    bool err = true;

    TYPE_SET_UNKNOWN(&f->type);
    resolve_expr_node(e->fils, f);
    fils = &f->type;

    /* ssi type primitif */
    if(fils->type_prim)
    {
	/* ssi on a 1 scalaire */
	if(fils->type.prim != PT_STRING)
	{
	    err = false;

	    f->type.type_prim = true;
	    f->type.type.prim = fils->type.prim;
	}
    }

    if(err)
	raise_error(ET_TYPE_UNA_ERR,
		    get_una_operator(e->type),
		    get_var_type(fils));

}

void resolve_constant_expr_node(cst_expr_node_t * n, resolve_env_t * f)
{
    c_assert(n && f);

    TYPE_SET_UNKNOWN(&f->type);

    switch(n->type)
    {
    case CNT_INT:
	f->type.type.prim = PT_INT;
	break;
    case CNT_DBL:
	f->type.type.prim = PT_REAL;
	break;
    case CNT_STR:
	f->type.type.prim = PT_STRING;
	break;
    case CNT_IDF:	
	c_warning2(false, "TODO");
	break;
    }
}

void resolve_rvalue_node(rvalue_node_t * n, resolve_env_t * f)
{
    c_assert(n && f);

    switch(n->type)
    {
    case RNT_NEW:
	resolve_new_expr_node(n->node.nnew, f);
	break;
    case RNT_EXPR:
	resolve_expr_node(n->node.expr, f);
	break;
    }
}

void resolve_call_expr_node(call_expr_node_t * c, resolve_env_t * f)
{
    c_assert(n);

    switch(n->type)
    {
    case CENT_DIRECT:
	resolve_direct_call_expr_node(n->node.dc, f);
	break;
    case CENT_MEMBER:
	resolve_member_expr_node(n->node.mem, f);
	break;
    }
}

void resolve_direct_call_expr_node(direct_call_expr_node_t * n, resolve_env_t * f)
{
    c_assert(n);

    var_type_t context = n->context;

    /* si pas de contexte alors on prend this */
    if(TYPE_IS_UNKNOWN(&context))
    {
	context.type_prim = false;
	context.type.uclass = f->current_class;
    }

    /* ssi on a un type primitif */
    if(context.type_prim)
    {
	raise_error(ET_TYPE_MBR_ERR, get_var_type(&context));
	return;
    }

    c_assert(context.type.uclass && !context.type_prim);

    tds_t * tds = context.type.uclass->tds;

    n->resolved = NULL;

    switch(n->type)
    {
    case DCENT_FN:
       {

	   vector_t * v = resolve_param_eff_list(n->node.fnc->params, f);

	   n->resolved = 
	       tds_search_function(tds, n->node.fnc->name, v, true);

	   free_vector(v);

	   if(!n->resolved)
	       raise_error(ET_FUNC_NOT_FOUND,
			   lexique_get(c_lexique, n->node.fnc->name));

	   break;
       }
    case DCENT_IDF:
       {
	   n->resolved = 
	       tds_search_from_index(tds, n->node.vidf, OBJ_VAR, true);

	   if(!n->resolved)
	       raise_error(ET_VAR_NO_DEC, lexique_get(c_lexique, n->node.vidf));

	   break;
       }
    }
}

vector_t * resolve_param_eff_list(param_eff_expr_node_t * n, resolve_env_t * f)
{
    c_assert(n && n->params);

    size_t i;
    size_t n = vector_size(n->params);
    vector_t * v = create_vector(n);

    for(i = 0; i < n; ++i)
    {
	
    }

    return v;
}

void resolve_member_expr_node(member_expr_node_t * n, resolve_env_t * f)
{
   /* operateur '.'  */

   c_assert(n && f);

   /* contexte = le type de la partie gauche */
    

    TYPE_SET_UNKNOWN(&f->type);
    resolve_call_expr_node(n->p, f);
    f->context = f->type;


    TYPE_SET_UNKNOWN(&f->type);
    resolve_direct_call_expr_node(n->f, f);

}


void resolve_new_expr_node(new_expr_node_t * n, resolve_env_t * f)
{
}


void resolve_instr_node(instr_node_t * n, resolve_env_t * f)
{
    c_assert(n);

    switch(n->type)
    {
    case INT_LOOP: resolve_loop_instr_node(n->node.loop, f); break;
    case INT_CALL: resolve_call_instr_node(n->node.call, f); break;
    case INT_COND: resolve_cond_instr_node(n->node.cond, f); break;
    case INT_RETURN: resolve_return_instr_node(n->node.ret, f); break;
    case INT_SUPER: resolve_super_instr_node(n->node.super, f); break;
    case INT_AFFECT: resolve_affect_instr_node(n->node.aff, f); break;
    }

}

void resolve_loop_instr_node(loop_instr_node_t * n, resolve_env_t * f)
{
    c_assert(n);

   /* pas la peine de verifier le type de la condition */

    resolve_expr_node(n->cond, f);
    resolve_bloc_instr_node(n->body, f);

    /* on peut pas garantir qu'on rentrera dans le boucle */
    f->contains_return = false;
}

void resolve_cond_instr_node(cond_instr_node_t * n, resolve_env_t * f)
{
    c_assert(n);

/* pas la peine de verifier le type de la condition */

    resolve_expr_node(n->cond, f);
    f->contains_return = false;

    resolve_bloc_instr_node(n->btrue, f);

    bool false_path_contains_return = false;
    bool true_path_contains_return = f->contains_return;

    if(n->bfalse)
    {
	f->contains_return = false;
	resolve_bloc_instr_node(n->bfalse, f);
	false_path_contains_return = f->contains_return;
    }

   /* contient un return si les deux chemins en contiennent un */
    f->contains_return = false_path_contains_return &&
	true_path_contains_return;

}

void resolve_call_instr_node(call_instr_node_t * n, resolve_env_t * f)
{
    c_assert(n);
    f->contains_return = false;
    resolve_call_expr_node(n->c, f);
}

void resolve_return_instr_node(return_instr_node_t * n, resolve_env_t * f)
{
    c_assert(n && f->current_fn);
    resolve_expr_node(n->expr, f);
    f->contains_return = true;

    if(f->current_fn->ret_type)
	resolve_var_type_assignement(&f->type, f->current_fn->ret_type);
}

void resolve_super_instr_node(super_instr_node_t * n, resolve_env_t * f)
{
    c_assert(n);
    f->contains_return = false;
    resolve_param_eff_expr_node(n->params, f);
}

void resolve_affect_instr_node(affect_instr_node_t * n, resolve_env_t * f)
{
    c_assert(n);
    f->contains_return = false;

    resolve_rvalue_node(n->rvalue, f);

    n->lvalue_resolved = resolve_var_type(n->lvalue, f);

    resolve_var_type_assignement(&f->type, n->lvalue_resolved->type);
}

void resolve_bloc_instr_node(bloc_instr_node_t * n, resolve_env_t * f)
{
    c_assert(n);

    f->contains_return = false;

    bool loop_return = false;

    tds_t * old_tds = f->current_tds;

    if(n->tds)
    {
	f->current_tds = n->tds;
	resolve_tds(n->tds, f);
    }

    int i;
    int s = count_instr_bloc(n);
    for(i = 0; i < s; ++i)
    {
	resolve_instr_node(get_bloc_instr(n, i), f);

        /* contient un return si au moins une instruction
	 du block en contient une */
	loop_return = loop_return || f->contains_return;
    }

    f->contains_return = loop_return;
    f->current_tds = old_tds;

}

void resolve_var_type_assignement(var_type_t * from, var_type_t * to)
{
    c_assert(from && to);

    if(!can_assign_var_type(from, to))
	raise_error(ET_TYPE_MISMATCH, get_var_type(from), 
		    get_var_type(to));

}

tds_entry_t * resolve_var_type(size_t index, resolve_env_t * f)
{
    tds_entry_t * r = 
	tds_search_from_index(f->current_tds, index, OBJ_VAR, true);

    if(!r)
	raise_error(ET_VAR_NO_DEC, lexique_get(c_lexique, index));

    return r;
}


void resolve_tds(tds_t * t, resolve_env_t * f)
{
    c_assert(t);
    size_t s = tds_count(t);
    size_t i;

    for(i = 0; i < s; ++i)
	resolve_tds_entry(tds_get_entry(t, i), f);
}

void resolve_tds_entry(tds_entry_t * t, resolve_env_t * f)
{
    c_assert(t);

    switch(t->otype)
    {
    case OBJ_CLASS:
	resolve_class_node(t->infos.cl, f);
	break;
    case OBJ_PROC:
    case OBJ_FUNC:
    case OBJ_CTOR:
	resolve_function_node(t->infos.fn, f);
	break;
    case OBJ_FIELD:
	/* rien a faire, deja resolu avant */
	break;
    case OBJ_PARAM:
    case OBJ_LOCAL_VAR:
	/* rien a faire */
	break;
    }

}

void resolve_class_node(class_node_t * cl, resolve_env_t * f)
{
    c_assert(cl);

    f->current_cl = cl;
    f->current_tds = cl->tds;
    yylineno = cl->line;

    /* super deja resolu */

    /* on verifie la presence d'un constructeur */
    if(!tds_search_from_index(cl->tds, CTOR_NAME, OBJ_CTOR, false))
	raise_error(ET_NO_CTOR_FOUND, lexique_get(c_lexique, cl->name_index));

    resolve_tds(cl->tds, f);
}

void resolve_function_node(function_node_t * cl, resolve_env_t * f)
{
    c_assert(cl);

    /* parametre deja resolu */

    yylineno = cl->line;
    f->current_fn = cl;
    f->contains_return = false;
    resolve_bloc_instr_node(cl->block, f);

    if(cl->ret_type) /* c'est une fonction */
    {
	if(!f->contains_return)
	    raise_error(ET_NO_RETURN_IN_FUNC, lexique_get(c_lexique, cl->name_index));
    }
    else /* c'est une procedure */
    {
	if(f->contains_return)
	    raise_error(ET_RETURN_IN_PROC, lexique_get(c_lexique, cl->name_index));
    }
    yylineno = -1;

}


void resolve_start(tree_base_t * b)
{
    yylineno = -1;

    resolve_env_t env;
    memset(&env, 0, sizeof(env));

    env.current_tds = b;
    TYPE_SET_UNKNOWN(&env.type);

    resolve_tds(b, &env);
}
