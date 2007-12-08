
#include "tds.h"
#include "assert.h"

extern lexique_t * c_lexique;


tds_entry_t * tds_search_from_name(tds_t * tds, char * name)
{
    c_assert(name);

    size_t r = lexique_search(c_lexique, name);

    /*
      si name n'est pas dans le lexique, 
      il y a aucune chance pour que ce soit
      un bon nom de variable.
     */
    if(r == KEY_NOT_FOUND)
	return NULL;
    else
	return tds_search_from_index(tds, r);
}

tds_entry_t * tds_search_from_index(tds_t * tds, size_t index)
{
    c_assert(tds);
    c_assert(index < lexique_count(c_lexique));

    size_t s = vector_size(tds->entries);
    size_t i;

    for(i = 0; i < s; ++i)
    {
	tds_entry_t * e = tds_get_entrie(tds, i);
	if(e->name_index == index)
	    return e;
    }

    if(tds->parent)
	return tds_search_from_index(tds->parent, index);
    else
	return NULL;
}

