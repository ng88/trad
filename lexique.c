
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

    l->size = 0;
    l->capacity = 32;
    l->table = (char**)malloc(l->capacity * sizeof(char*));
    l->hashtbl = create_hashtable(l->capacity, lexique_str_hash,
				  lexique_str_equals);

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

    if(l->size == l->capacity)
    {
	l->capacity *= 2;
	l->table = realloc(l->table, l->capacity * sizeof(char*));

	c_assert2(l->table, "realloc failed");
    }

    l->table[l->size] = str;

    size_t * value = (size_t*)malloc(sizeof(size_t));
    *value = l->size;
    
    c_assert2(value, "malloc failed");

    _lexique_insert(l->hashtbl, str, value);

    l->size++;
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
    c_assert(index < lexique_count(l));
    return l->table[index];
}

size_t lexique_count(lexique_t * l)
{
    c_assert(l);
    c_assert(l->size == hashtable_count(l->hashtbl));
    return l->size;
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
