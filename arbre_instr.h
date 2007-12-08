
#ifndef ARBRE_INSTR_H
#define ARBRE_INSTR_H

#include <stdio.h>
#include "arbre_expr.h"
#include "tds.h"

/** Declarations pour les arbres d'instructions
 */

/** Types de noeuds */
typedef enum
{
    INT_LOOP,
    INT_CALL,
    INT_COND,
    INT_RETURN,
    INT_SUPER,
    INT_AFFECT,
} instr_node_type_t;



struct _loop_instr_node_t;
struct _call_instr_node_t;
struct _cond_instr_node_t;
struct _return_instr_node_t;
struct _super_instr_node_t;
struct _affect_instr_node_t;

/** Noeud */
typedef struct _instr_node_t
{
    instr_node_type_t type;
    union
    {
	struct _loop_instr_node_t * loop;
	struct _call_instr_node_t * call;
	struct _cond_instr_node_t * cond;
	struct _return_instr_node_t * ret;
	struct _super_instr_node_t * super;
	struct _affect_instr_node_t * aff;
    } node;
} instr_node_t;

typedef struct _call_instr_node_t
{
    call_expr_node_t * c;
} call_instr_node_t;

typedef struct _return_instr_node_t
{
    expr_node_t * expr;
} return_instr_node_t;

typedef struct _super_instr_node_t
{
    param_eff_expr_node_t * params;
} super_instr_node_t;


typedef struct _affect_instr_node_t
{
    idf_t           lvalue;
    rvalue_node_t * rvalue;
} affect_instr_node_t;

/** Bloc d'instruction */
typedef struct _bloc_instr_node_t
{
    /** Table des symboles*/
    tds_t * tds;

    /** Liste des instructions, moins lourd qu'une liste chainee 
	si on suppose qu'il n'y aura pas enormement d'instruction
	par bloc
     */
    vector_t * instrs;

} bloc_instr_node_t;


typedef struct _cond_instr_node_t
{
    /** condition */
    expr_node_t * cond;

    /** bloc si vrai */
    bloc_instr_node_t * btrue;

    /** bloc si faux (eventuellement NULL) */
    bloc_instr_node_t * bfalse;

} cond_instr_node_t;

typedef struct _loop_instr_node_t
{
    /** condition */
    expr_node_t * cond;

    bloc_instr_node_t * body;

} loop_instr_node_t;


/** Creation des noeuds d'instructions
 */
instr_node_t * make_instr_node(instr_node_type_t t);

instr_node_t * make_loop_instr_node(expr_node_t * c, bloc_instr_node_t * b);
instr_node_t * make_cond_instr_node(expr_node_t * c, bloc_instr_node_t * bt, 
				    bloc_instr_node_t * bf);
instr_node_t * make_call_instr_node(call_expr_node_t * c);
instr_node_t * make_return_instr_node(expr_node_t * e);
instr_node_t * make_super_instr_node(param_eff_expr_node_t * p);
instr_node_t * make_affect_instr_node(idf_t lv, rvalue_node_t * rv);

bloc_instr_node_t * make_bloc_instr_node();
void add_instr_bloc(bloc_instr_node_t * b, instr_node_t * i);
size_t count_instr_bloc(bloc_instr_node_t * b);
instr_node_t * get_bloc_instr(bloc_instr_node_t * b, size_t index);



/** Destruction des noeuds d'instructions
 */
void free_instr_node(instr_node_t * n);

void free_loop_instr_node(loop_instr_node_t * n);
void free_cond_instr_node(cond_instr_node_t * n);
void free_call_instr_node(call_instr_node_t * n);
void free_return_instr_node(return_instr_node_t * n);
void free_super_instr_node(super_instr_node_t * n);
void free_affect_instr_node(affect_instr_node_t* n);

void free_bloc_instr_node(bloc_instr_node_t * n);

#endif
