
#include <string.h>
#include "lexique.h"
#include "assert.h"

DEFINE_HASHTABLE_INSERT(_lexique_insert, char, size_t);
DEFINE_HASHTABLE_SEARCH(_lexique_search, char, size_t);
DEFINE_HASHTABLE_REMOVE(_lexique_remove, char, size_t);

lexique_t * create_lexique()
{
    lexique_t * l = (lexique_t *)malloc(sizeof(lexique_t));

    c_assert2(l, "malloc failed");

    l->tbl = create_vector(32);

    l->hashtbl = create_hashtable(32, lexique_str_hash,
				  lexique_str_equals);

    c_assert(l->hashtbl);

    return l;
}

void free_lexique(lexique_t * l)
{
    if(!l) return;

    free_vector(l->tbl, 0);
    hashtable_destroy(l->hashtbl, 1);
    free(l);
}

size_t lexique_add_sole(lexique_t * l, char * str)
{
    size_t pos = lexique_search(l, str);
                                            
    if(pos == KEY_NOT_FOUND)
    {
	pos = lexique_count(l);
	lexique_add(l, str);
    }
    else
	free(str);

    return pos;
}

void lexique_add(lexique_t * l, char * str)
{
    c_assert(l && str);

    size_t * value = (size_t*)malloc(sizeof(size_t));
    *value = vector_size(l->tbl);

    c_assert2(value, "malloc failed");

    _lexique_insert(l->hashtbl, str, value);

    vector_add_element(l->tbl, str);

}

size_t lexique_search(lexique_t * l, char * str)
{
    c_assert(l && str);

    size_t * i;

    if( !(i = _lexique_search(l->hashtbl, str)) )
	return KEY_NOT_FOUND;

    return *i;

}

char* lexique_get(lexique_t * l, size_t index)
{
    c_assert(l);

    return (char*)vector_get_element_at(l->tbl, index);
}

size_t lexique_count(lexique_t * l)
{
    c_assert(l);
    c_assert(vector_size(l->tbl) == hashtable_count(l->hashtbl));

    return vector_size(l->tbl);
}

unsigned int lexique_str_hash(void * v)
{
    unsigned int hash = 5381;
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
