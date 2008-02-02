
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


#endif

