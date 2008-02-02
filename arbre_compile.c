
#include <stdlib.h>

#include "arbre_compile.h"

#include "assert.h"
#include "lexique.h"

#include "error.h"

extern lexique_t * c_lexique;


compile_env_t * make_compile_env(char * dest)
{
    FILE * f = fopen(dest, "w");

    if(!f)
	return NULL;

    compile_env_t * c =
	(compile_env_t*)malloc(sizeof(compile_env_t));

    c_assert(c);

    c->dest = f;

    return c;
}

void free_compile_env(compile_env_t * e)
{
    c_assert(e);

    fclose(e->dest);

    free(e);
}

void compile_start(compile_env_t * e, tree_base_t * b, function_node_t * entry_point)
{
    c_assert(e && b);

    if(!entry_point)
	raise_error(ET_MAIN_NOT_FOUND);


    

}









