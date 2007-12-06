
#ifndef LEXIQUE_H
#define LEXIQUE_H

#include "hashtable/hashtable.h"
#include "vector/vector.h"

#define KEY_NOT_FOUND ((size_t)-1)

typedef struct
{
    /** Pour une recherche efficace*/
    struct hashtable * hashtbl;

    /** Pour un accès rapide */
    vector_t * tbl;

} lexique_t;


lexique_t * create_lexique();
void free_lexique(lexique_t * l);

size_t lexique_add_sole(lexique_t * l, char * str);

void lexique_add(lexique_t * l, char * str);
size_t lexique_search(lexique_t * l, char * str);
char* lexique_get(lexique_t * l, size_t index);
size_t lexique_count(lexique_t * l);

unsigned int lexique_str_hash(void * str);
int lexique_str_equals(void * k1, void * k2);

#endif
