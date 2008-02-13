
#ifndef ARBRE_CLASSE_H
#define ARBRE_CLASSE_H

#include "arbre_instr.h"
#include "tds.h"

/** Declarations pour les arbres les classes.
 */

#define CTOR_NAME 0

typedef struct _class_node_t
{
    struct _class_node_t * super;

    size_t name_index;

    tds_t * tds;

    int line;

} class_node_t;



typedef struct _function_node_t
{
    class_node_t * parent;

    scope_t scope;

    size_t name_index;

    bloc_instr_node_t * block;

    var_type_t * ret_type;

    vector_t * params;

    int line;

} function_node_t;

typedef tds_t tree_base_t;



function_node_t * make_function_node(size_t name_index,
				     scope_t scope,
				     vector_t * params,
				     bloc_instr_node_t * block);

function_node_t * make_procedure_node(size_t name_index,
				      scope_t scope,
				      vector_t *  params,
				      bloc_instr_node_t * block);

function_node_t * make_constructor_node(scope_t scope,
					vector_t *  params,
					bloc_instr_node_t * block);

class_node_t * make_class_node(size_t name_index,
			       tds_t * parent);

void free_function_node(function_node_t *e);
void free_class_node(class_node_t * e);


#endif
