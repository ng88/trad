
#ifndef ARBRE_SEM_H
#define ARBRE_SEM_H


#include "arbre.h"

/** Declare les fonctions de verification sematique de l'arbre */

/**
   Grace au fait que l'on n'est pas de reference en avant, la plupart
   des resolutions est deja faite dans la passe d'avant.
*/

/** Environnement de resolution semantique
 */
typedef struct
{
    class_node_t * current_cl;
    function_node_t * current_fn;
    tds_t * current_tds;

    var_type_t type;
    var_type_t context;

    bool contains_return;

} resolve_env_t;

void resolve_expr_node(expr_node_t * e, resolve_env_t * f);

void resolve_binary_expr_node(bin_expr_node_t * e, resolve_env_t * f);
void resolve_unary_expr_node(una_expr_node_t * e, resolve_env_t * f);
void resolve_constant_expr_node(cst_expr_node_t * e, resolve_env_t * f);

void resolve_new_expr_node(new_expr_node_t * n, resolve_env_t * f);
void resolve_rvalue_node(rvalue_node_t * n, resolve_env_t * f);

void resolve_call_expr_node(call_expr_node_t * c, resolve_env_t * f);
void resolve_direct_call_expr_node(direct_call_expr_node_t * n, resolve_env_t * f);
void resolve_member_expr_node(member_expr_node_t * n, resolve_env_t * f);

void resolve_instr_node(instr_node_t * n, resolve_env_t * f);

void resolve_loop_instr_node(loop_instr_node_t * n, resolve_env_t * f);
void resolve_cond_instr_node(cond_instr_node_t * n, resolve_env_t * f);
void resolve_call_instr_node(call_instr_node_t * n, resolve_env_t * f);
void resolve_return_instr_node(return_instr_node_t * n, resolve_env_t * f);
void resolve_super_instr_node(super_instr_node_t * n, resolve_env_t * f);
void resolve_affect_instr_node(affect_instr_node_t* n, resolve_env_t * f);

void resolve_bloc_instr_node(bloc_instr_node_t * n, resolve_env_t * f);

void resolve_tds(tds_t * t, resolve_env_t * f);
void resolve_tds_entry(tds_entry_t * t, resolve_env_t * f);


void resolve_class_node(class_node_t * cl, resolve_env_t * f);
void resolve_function_node(function_node_t * cl, resolve_env_t * f);

void resolve_start(tree_base_t * b);


void resolve_var_type_assignement(var_type_t * from, var_type_t * to);

tds_entry_t * resolve_var_type(size_t index, resolve_env_t * f);

vector_t * resolve_param_eff_list(param_eff_expr_node_t * n, resolve_env_t * f);


#endif


