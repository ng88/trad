
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
struct _bloc_instr_node_t;

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


typedef struct _loop_instr_node_t
{
} loop_instr_node_t;

typedef struct _call_instr_node_t
{
    call_expr_node_t * c;
} call_instr_node_t;

typedef struct _cond_instr_node_t
{
} cond_instr_node_t;

typedef struct _return_instr_node_t
{
    expr_node_t * expr;
} return_instr_node_t;

typedef struct _super_instr_node_t
{
} super_instr_node_t;


typedef struct _affect_instr_node_t
{
    idf_t           lvalue;
    rvalue_node_t * rvalue;
} affect_instr_node_t;

/** element de la liste du bloc d'instruction*/
typedef struct _bloc_list_elt_t
{
    instr_node_t * instr;
    struct _bloc_list_elt_t * next;
} bloc_list_elt_t;

/** Bloc d'instruction */
typedef struct _bloc_instr_node_t
{
    /** Table des symboles*/
    tds_t * tds;

    /** Premier noeud de la liste chainee des instructions */
    bloc_list_elt_t * first;

} bloc_instr_node_t;


#endif
