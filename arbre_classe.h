
#ifndef ARBRE_CLASSE_H
#define ARBRE_CLASSE_H

#include "arbre_instr.h"
#include "tds.h"

/** Declarations pour les arbres les classes.
 */


typedef struct _class_node_t
{
    struct _class_node_t * super;

    size_t name_index;

    tds_t * tds;

} class_node_t;



typedef struct _function_node_t
{
    class_node_t * parent;

    scope_t scope;

    size_t name_index;

    bloc_instr_node_t * block;

    var_type_t * ret_type;
    vector_t * params;

} function_node_t;




function_node_t * make_function_node(size_t name_index, scope_t scope);
class_node_t * make_class_node(size_t name_index, tds_t * parent);

void free_function_node(function_node_t *e);
void free_class_node(class_node_t * e);


#endif
