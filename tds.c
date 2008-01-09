
#include "tds.h"
#include "assert.h"

extern lexique_t * c_lexique;



tds_t * make_tds(tds_t * parent)
{
    tds_t * r = (tds_t *)malloc(sizeof(tds_t));
    c_assert2(r, "malloc failed");

    r->parent = parent;
    r->entries = create_vector(4);

    return r;
}

void tds_add_entry(tds_t * t, tds_entry_t * e)
{
    c_assert(t && e);

    vector_add_element(t->entries, e);
}

void tds_add_entries(tds_t * tds, vector_t * indices, var_type_t *t, object_type_t ot)
{
    c_assert(tds && indices && t);
    size_t s = vector_size(indices);
    size_t i;
    t->ref += s - 1;
    for(i = 0; i < s; ++i)
    {
	size_t index = UNBOX_UINT(vector_get_element_at(indices, i));
	tds_add_entry(tds, make_tds_entry(index, t, ot));
    }

}


tds_entry_t * make_tds_entry(size_t name_index, var_type_t * t, object_type_t ot)
{
    c_assert2(name_index < lexique_count(c_lexique), "invalid index");

    tds_entry_t * r = (tds_entry_t *)malloc(sizeof(tds_entry_t));
    c_assert2(r, "malloc failed");

    r->name_index = name_index;
    r->type = t;
    r->otype = ot;
    r->infos.fn = NULL;

    return r;
}

function_infos_t * make_function_infos(class_node_t * cl)
{
    function_infos_t * r = (function_infos_t *)malloc(sizeof(function_infos_t));
    c_assert2(r, "malloc failed");

    r->cl = cl;
    r->params = create_vector(2);
    r->return_type = NULL;

    return r;
}

proc_infos_t * make_proc_infos(class_node_t * cl)
{
    proc_infos_t * r = (proc_infos_t *)malloc(sizeof(proc_infos_t));
    c_assert2(r, "malloc failed");

    r->cl = cl;
    r->params = create_vector(2);

    return r;
}

class_infos_t * make_class_infos()
{
    class_infos_t * r = (class_infos_t *)malloc(sizeof(class_infos_t));
    c_assert2(r, "malloc failed");

    return r;
}

var_type_t * make_var_type(bool type_prim)
{
    var_type_t * r = (var_type_t *)malloc(sizeof(var_type_t));
    c_assert2(r, "malloc failed");
    r->type_prim = type_prim;
    r->ref = 1;
    return r;
}

var_type_t * make_var_prim_type(primitive_type_t t)
{
    var_type_t * r = make_var_type(true);
    r->type.prim = t;
    return r;
}

var_type_t * make_var_user_type(size_t idf)
{
    var_type_t * r = make_var_type(false);
    r->type.uclass = NULL;
    c_warning2(0, "TODO recuperation effective du type");
    return r;
}


void free_function_infos(function_infos_t *e)
{
    c_assert(e);
    free_vector(e->params, 0);
    free(e);
}

void free_proc_infos(proc_infos_t *e)
{
    c_assert(e);
    free_vector(e->params, 0);
    free(e);
}

void free_class_infos(class_infos_t * e)
{
    c_assert(e);
    free(e);
}

void free_var_type(var_type_t * t)
{
    c_assert(t);
    t->ref--;

    if(t->ref == 0)
	free(t);
}

void free_tds(tds_t * t)
{
    c_assert(t);

    size_t i;
    size_t s = vector_size(t->entries);

    for(i = 0; i < s; ++i)
	free_tds_entry(tds_get_entry(t, i));

    free_vector(t->entries, 0);

    free(t);
}

void free_tds_entry(tds_entry_t * e)
{

    c_assert(e);

    switch(e->otype)
    {
    case OBJ_CLASS: free_class_infos(e->infos.cl); break;
    case OBJ_PROC: free_proc_infos(e->infos.pr); break;
    case OBJ_FUNC:free_function_infos(e->infos.fn); break;
    case OBJ_LOCAL_VAR: break;
    }

    free_var_type(e->type);

    free(e);
}

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
	tds_entry_t * e = tds_get_entry(tds, i);
	if(e->name_index == index)
	    return e;
    }

    if(tds->parent)
	return tds_search_from_index(tds->parent, index);
    else
	return NULL;
}

