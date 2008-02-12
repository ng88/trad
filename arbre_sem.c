
#include <stdlib.h>

#include "assert.h"
#include "arbre_sem.h"
#include "error.h"

#include "lexique.h"

extern lexique_t * c_lexique;


void resolve_expr_node(expr_node_t * e, resolve_env_t * f);

void resolve_binary_expr_node(bin_expr_node_t * e, resolve_env_t * f);
void resolve_unary_expr_node(una_expr_node_t * e, resolve_env_t * f);
void resolve_constant_expr_node(cst_expr_node_t * e, resolve_env_t * f);

void resolve_param_eff_expr_node(param_eff_expr_node_t * n, resolve_env_t * f);
void resolve_new_expr_node(new_expr_node_t * n, resolve_env_t * f);
void resolve_rvalue_node(rvalue_node_t * n, resolve_env_t * f);

void resolve_call_expr_node(call_expr_node_t * c, resolve_env_t * f);
void resolve_direct_call_expr_node(direct_call_expr_node_t * n, resolve_env_t * f);
void resolve_member_expr_node(member_expr_node_t * n, resolve_env_t * f);
void resolve_fn_call_expr_node(fn_call_expr_node_t * n, resolve_env_t * f);


void resolve_instr_node(instr_node_t * n, resolve_env_t * f);

void resolve_loop_instr_node(loop_instr_node_t * n, resolve_env_t * f);
void resolve_cond_instr_node(cond_instr_node_t * n, resolve_env_t * f);
void resolve_call_instr_node(call_instr_node_t * n, resolve_env_t * f);
void resolve_return_instr_node(return_instr_node_t * n, resolve_env_t * f);
void resolve_super_instr_node(super_instr_node_t * n, resolve_env_t * f);
void resolve_affect_instr_node(affect_instr_node_t* n, resolve_env_t * f);

void resolve_bloc_instr_node(bloc_instr_node_t * n, resolve_env_t * f);

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

}


void resolve_type_list(vector_t * params, resolve_env_t * f);
void resolve_scope(scope_t s, resolve_env_t * f);
