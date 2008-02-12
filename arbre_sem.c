
#include <stdlib.h>

#include "assert.h"
#include "arbre_sem.h"
#include "error.h"

#include "lexique.h"

extern lexique_t * c_lexique;


void resolve_expr_node(expr_node_t * e, resolve_env_t * f)
{
}

void resolve_binary_expr_node(bin_expr_node_t * e, resolve_env_t * f);
void resolve_unary_expr_node(una_expr_node_t * e, resolve_env_t * f);
void resolve_constant_expr_node(cst_expr_node_t * e, resolve_env_t * f);

void resolve_param_eff_expr_node(param_eff_expr_node_t * n, resolve_env_t * f)
{
}

void resolve_new_expr_node(new_expr_node_t * n, resolve_env_t * f);
void resolve_rvalue_node(rvalue_node_t * n, resolve_env_t * f)
{
}

void resolve_call_expr_node(call_expr_node_t * c, resolve_env_t * f)
{
}

void resolve_direct_call_expr_node(direct_call_expr_node_t * n, resolve_env_t * f);
void resolve_member_expr_node(member_expr_node_t * n, resolve_env_t * f);
void resolve_fn_call_expr_node(fn_call_expr_node_t * n, resolve_env_t * f);


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

    resolve_expr_node(n->cond, f);
    resolve_bloc_instr_node(n->body, f);

    /* on peut pas garantir qu'on rentrera dans le boucle */
    f->contains_return = false;
}

void resolve_cond_instr_node(cond_instr_node_t * n, resolve_env_t * f)
{
    c_assert(n);

//verif ici
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
    c_assert(n);
    resolve_expr_node(n->expr, f);
    f->contains_return = true;
//verif ici
}

void resolve_super_instr_node(super_instr_node_t * n, resolve_env_t * f)
{
    c_assert(n);
    f->contains_return = false;
    resolve_param_eff_expr_node(n->params, f);
}

void resolve_affect_instr_node(affect_instr_node_t* n, resolve_env_t * f)
{
    c_assert(n);
    f->contains_return = false;
//verif ici
    resolve_rvalue_node(n->rvalue, f);
}

void resolve_bloc_instr_node(bloc_instr_node_t * n, resolve_env_t * f)
{
    c_assert(n);

    f->contains_return = false;

    bool loop_return = false;

    if(n->tds)
	resolve_tds(n->tds, f);

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

void resolve_var_type(var_type_t * t, resolve_env_t * f);

void resolve_class_node(class_node_t * cl, resolve_env_t * f)
{
    c_assert(cl);

    f->current_cl = cl;

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



}

void resolve_type_list(vector_t * params, resolve_env_t * f);
void resolve_scope(scope_t s, resolve_env_t * f);


void resolve_start(tree_base_t * b)
{
    resolve_env_t env;
    resolve_tds(b, &env);
}
