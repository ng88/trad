
#ifndef LEXIQUE_H
#define LEXIQUE_H

#include "hashtable/hashtable.h"

#define KEY_NOT_FOUND ((size_t)-1)

DEFINE_HASHTABLE_INSERT(_lexique_insert, char, size_t);
DEFINE_HASHTABLE_SEARCH(_lexique_search, char, size_t);
DEFINE_HASHTABLE_REMOVE(_lexique_remove, char, size_t);

typedef struct
{
    /** Pour une recherche efficace*/
    struct hashtable * hashtbl;

    /** Pour un accès rapide */
    char ** table;
    size_t size;
    size_t capacity;
} lexique_t;


lexique_t * create_lexique();
void free_lexique(lexique_t * l);

void lexique_add(lexique_t * l, char * str);
size_t lexique_search(lexique_t * l, char * str);
char* lexique_get(lexique_t * l, size_t index);
size_t lexique_count(lexique_t * l);

unsigned int lexique_str_hash(void * str);
int lexique_str_equals(void * k1, void * k2);

#endif
