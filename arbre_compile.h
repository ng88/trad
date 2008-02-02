
#ifndef ARBRE_COMPILE_H
#define ARBRE_COMPILE_H

#include <stdio.h>
#include "arbre.h"

/** Declare les fonctions permettant de faire la compilation arbre -> C */


/** Environnement de compilation
 */
typedef struct
{
    /** Fichier de destination */
    FILE * dest;

    int indent;

    /** Options de compilation */
    
} compile_env_t;


compile_env_t * make_compile_env(char * dest);
void free_compile_env(compile_env_t * e);

void compile_start(compile_env_t * e, tree_base_t * b, function_node_t * entry_point);




void compile_tds(compile_env_t * e, tds_t * t);
void compile_tds_entry(compile_env_t * e, tds_entry_t * t);


void compile_field(compile_env_t * e, class_node_t * cl);

void compile_class_node(compile_env_t * e, class_node_t * cl);
void compile_function_node(compile_env_t * e, function_node_t * fn);

void compile_var_type(compile_env_t * e, var_type_t * t);

void compile_type_list(compile_env_t * e, function_node_t * fn);

void compile_bloc_instr_node(compile_env_t * e, bloc_instr_node_t * n);

void compile_instr_node(compile_env_t * e, instr_node_t * n);
void compile_loop_instr_node(compile_env_t * e, loop_instr_node_t * n);
void compile_call_instr_node(compile_env_t * e, call_instr_node_t * n);
void compile_cond_instr_node(compile_env_t * e, cond_instr_node_t * n);
void compile_return_instr_node(compile_env_t * e, return_instr_node_t * n);
void compile_super_instr_node(compile_env_t * e, super_instr_node_t * n);
void compile_affect_instr_node(compile_env_t * e, affect_instr_node_t* n);

void compile_expr_node(compile_env_t * e, expr_node_t * n);
void compile_binary_expr_node(compile_env_t * e, bin_expr_node_t * n);
void compile_unary_expr_node(compile_env_t * e, una_expr_node_t * n);
void compile_constant_expr_node(compile_env_t * e, cst_expr_node_t * n);
void compile_new_expr_node(compile_env_t * e, new_expr_node_t * n);
void compile_rvalue_node(compile_env_t * e, rvalue_node_t * n);

void compile_call_expr_node(compile_env_t * e, call_expr_node_t * n);
void compile_direct_call_expr_node(compile_env_t * e, direct_call_expr_node_t * n);
void compile_member_expr_node(compile_env_t * e, member_expr_node_t * n);
void compile_fn_call_expr_node(compile_env_t * e, fn_call_expr_node_t * n);


#endif
