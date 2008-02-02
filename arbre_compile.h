
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



#endif

