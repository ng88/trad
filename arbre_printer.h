
#ifndef ARBRE_PRINTER_H
#define ARBRE_PRINTER_H


#include "arbre.h"

/** Declare les fonctions d'affichage de l'arbre */



/** Affiche un noeud d'expression */
void print_expr_node(expr_node_t * e, FILE * f);

void print_binary_expr_node(bin_expr_node_t * e, FILE * f);
void print_unary_expr_node(una_expr_node_t * e, FILE * f);
void print_constant_expr_node(cst_expr_node_t * e, FILE * f);

void print_param_eff_expr_node(param_eff_expr_node_t * n, FILE * f);
void print_new_expr_node(new_expr_node_t * n, FILE * f);
void print_rvalue_node(rvalue_node_t * n, FILE * f);

void print_call_expr_node(call_expr_node_t * c, FILE * f);
void print_direct_call_expr_node(direct_call_expr_node_t * n, FILE * f);
void print_member_expr_node(member_expr_node_t * n, FILE * f);
void print_fn_call_expr_node(fn_call_expr_node_t * n, FILE * f);

/** Affichage des noeuds d'instructions
 */
void print_instr_node(instr_node_t * n, FILE * f, int indent);

void print_loop_instr_node(loop_instr_node_t * n, FILE * f, int indent);
void print_cond_instr_node(cond_instr_node_t * n, FILE * f, int indent);
void print_call_instr_node(call_instr_node_t * n, FILE * f, int indent);
void print_return_instr_node(return_instr_node_t * n, FILE * f, int indent);
void print_super_instr_node(super_instr_node_t * n, FILE * f, int indent);
void print_affect_instr_node(affect_instr_node_t* n, FILE * f, int indent);

void print_bloc_instr_node(bloc_instr_node_t * n, FILE * f, int indent);

void print_indent(FILE * f, int indent);

/** Affichage TDS
 */
void print_tds(tds_t * t, FILE * f, int indent);
void print_tds_entry(tds_entry_t * t, FILE * f, int indent);
void print_var_type(var_type_t * t, FILE * f);

#endif
