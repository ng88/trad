
#include <string.h>
#include "lexique.h"
#include "assert.h"

lexique_t * create_lexique()
{
    lexique_t * l = (lexique_t *)malloc(sizeof(lexique_t));

    c_assert2(l, "malloc failed");

    l->size = 50;
    l->table = (char**)malloc(l->size * sizeof(char*));
    l->hashtbl = create_hashtable(50, lexique_str_hash, lexique_str_equals);

    c_assert2(l->table, "malloc failed");
    c_assert(l->hashtbl);

    return l;
}

void free_lexique(lexique_t * l)
{
    if(!l) return;

    free(l->table);
    hashtable_destroy(l->hashtbl, 1);
    free(l);
}

void lexique_add(lexique_t * l, char * str)
{
    c_assert(l && str);
}

int lexique_search(lexique_t * l, char * str)
{
    c_assert(l && str);
}

char* lexique_get(lexique_t * l, size_t index)
{
    c_assert(l);
    c_assert(index >= 0 && index < lexique_count(l));
    return l->table[index];
}

size_t lexique_count(lexique_t * l)
{
    c_assert(l);
    c_assert(l->size == hashtable_count(l->hashtbl));
    return l->size;
}

unsigned long lexique_str_hash(void * v)
{
    unsigned long hash = 5381;
    int c;

    unsigned char * str = (unsigned char *)v;
    
    while( (c = *str++) )
	hash = ((hash << 5) + hash) + c;
    
    return hash;
}

int lexique_str_equals(void * k1, void * k2)
{
    return !strcmp( (char*)k1, (char*)k2 );
}
