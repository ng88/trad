
#include "tds.h"
#include "assert.h"
#include "arbre.h"
#include "error.h"

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

	if(tds_search_from_index(tds, index, ot, false))
	    raise_error(ET_LVAR_ALREADY_EXISTS, lexique_get(c_lexique, index));

	tds_add_entry(tds, make_tds_entry(index, t, ot));
    }

}

void tds_add_field_entries(tds_t * tds,vector_t * indices, var_type_t *t,scope_t scope)
{
    c_assert(tds && indices && t);
    size_t s = vector_size(indices);
    size_t i;
    t->ref += s - 1;
    for(i = 0; i < s; ++i)
    {
	size_t index = UNBOX_UINT(vector_get_element_at(indices, i));

	if(tds_search_from_index(tds, index, OBJ_FIELD, false))
	    raise_error(ET_FIELD_ALREADY_EXISTS, lexique_get(c_lexique, index));

	tds_add_entry(tds, make_tds_field_entry(index, t, scope));
    }
}


tds_entry_t * make_tds_class_entry(struct _class_node_t * cl)
{
    c_assert(cl);

    tds_entry_t * e = make_tds_entry(cl->name_index, NULL, OBJ_CLASS);
    e->infos.cl = cl;

    return e;
}

tds_entry_t * make_tds_function_entry(struct _function_node_t * fn)
{
    c_assert(fn);

    tds_entry_t * e = make_tds_entry(fn->name_index, NULL, OBJ_FUNC);
    e->infos.fn = fn;

    return e;
}

tds_entry_t * make_tds_procedure_entry(struct _function_node_t * fn)
{
    c_assert(fn);

    tds_entry_t * e = make_tds_entry(fn->name_index, NULL, OBJ_PROC);
    e->infos.fn = fn;

    return e;
}

tds_entry_t * make_tds_constructor_entry(struct _function_node_t * fn)
{
    c_assert(fn);

    tds_entry_t * e = make_tds_entry(fn->name_index, NULL, OBJ_CTOR);
    e->infos.fn = fn;

    return e;
}



tds_entry_t * make_tds_field_entry(size_t name_index, var_type_t *t, scope_t scope)
{
    c_assert(t);

    tds_entry_t * e = make_tds_entry(name_index, t, OBJ_FIELD);
    e->infos.field_scope = scope;

    return e;
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

var_type_t * make_var_user_type(tds_t * t, size_t idf)
{
    var_type_t * r = make_var_type(false);
    r->type.uclass = resolve_class_identifier(t, idf);
    return r;
}

struct _class_node_t * entry_get_class(tds_entry_t * e)
{
    return e ? e->infos.cl : NULL;
}

struct _function_node_t * entry_get_function(tds_entry_t * e)
{
    return e ? e->infos.fn : NULL;
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
    size_t s = tds_count(t);

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
    case OBJ_CLASS: free_class_node(e->infos.cl); break;
    case OBJ_CTOR:
    case OBJ_PROC:
    case OBJ_FUNC: free_function_node(e->infos.fn); break;
    case OBJ_PARAM:
    case OBJ_LOCAL_VAR: break;
    case OBJ_FIELD: break;
    }

    if(e->type)
	free_var_type(e->type);

    free(e);
}

tds_entry_t * tds_search_from_name(tds_t * tds, char * name, object_type_t ot_mask, bool rec)
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
	return tds_search_from_index(tds, r, ot_mask, rec);
}

tds_entry_t * tds_search_from_index(tds_t * tds, size_t index, object_type_t ot_mask, bool rec)
{
    c_assert(tds);
    c_assert(index < lexique_count(c_lexique));

    size_t s = vector_size(tds->entries);
    size_t i;

    for(i = 0; i < s; ++i)
    {
	tds_entry_t * e = tds_get_entry(tds, i);
	if((e->otype & ot_mask) && e->name_index == index)
	    return e;
    }

    if(rec && tds->parent)
	return tds_search_from_index(tds->parent, index, ot_mask, true);
    else
	return NULL;
}

tds_entry_t * tds_search_function(tds_t * tds, size_t index, vector_t * params, bool rec)
{
    c_assert(tds);
    c_assert(index < lexique_count(c_lexique));

    size_t s = vector_size(tds->entries);
    size_t i;

    for(i = 0; i < s; ++i)
    {
	tds_entry_t * e = tds_get_entry(tds, i);

	if((e->otype & OBJ_FNP) && e->name_index == index
	   && param_equals(params, e->infos.fn->params))
	    return e;

    }

    if(rec && tds->parent)
	return tds_search_function(tds->parent, index, params, true);
    else
	return NULL;
}

bool param_equals(vector_t * p1, vector_t * p2)
{
    c_assert(p1 && p2);

    size_t n1 = vector_size(p1);
    size_t n2 = vector_size(p2);

    if(n1 != n2)
	return false;

    size_t i;

    for(i = 0; i < n1; ++i)
    {
	param_dec_t * e1 =
	    (param_dec_t *)vector_get_element_at(p1, i);
	param_dec_t * e2 =
	    (param_dec_t *)vector_get_element_at(p2, i);

	if( !var_type_equals(e1->type, e2->type) )
	    return false;
    }

    return true;
}

bool var_type_equals(var_type_t * v1, var_type_t * v2)
{
    c_assert(v1 && v2);

    if(v1->type_prim && v2->type_prim)
	return  v1->type.prim == v2->type.prim;
    else if(!v1->type_prim && !v2->type_prim)
	return  v1->type.uclass == v2->type.uclass;
    else
	return false;
}

bool can_assign_var_type(var_type_t * v1, var_type_t * v2)
{
    c_warning("TODO");

    if(TYPE_IS_UNKNOWN(v1) || TYPE_IS_UNKNOWN(v2))
	return false;

    return var_type_equals(v1, v2);
}

char * get_var_type(var_type_t * t)
{
    c_assert(t);

    char * st = "???";

    if(t->type_prim)
    {
	switch(t->type.prim)
	{
	case PT_STRING: st = "string"; break;
	case PT_INT: st = "integer"; break;
	case PT_REAL: st = "real"; break;
	case PT_UNKNOW: st = "<unknow>"; break;
	}
    }
    else
    {
	c_assert(t->type.uclass);
	st = lexique_get(c_lexique, t->type.uclass->name_index);
    }

    return st;

}

struct _class_node_t * resolve_class_identifier(tds_t * tds, size_t name_index)
{
    c_assert(tds);

    struct _class_node_t * r =
	entry_get_class(tds_search_from_index(tds, name_index, OBJ_CLASS, true));

    if(!r)
	raise_error(ET_CLASS_NOT_FOUND, lexique_get(c_lexique, name_index));

    return r;
}


param_dec_t * make_param_dec(size_t name, var_type_t * t)
{
    c_assert(t);

    param_dec_t * p = (param_dec_t *)malloc(sizeof(param_dec_t));

    c_assert(p);

    p->type = t;
    p->type->ref++;
    p->name_index = name;

    return p;
}


void free_param_dec(param_dec_t * p)
{
    c_assert(p);

    free_var_type(p->type);
    free(p);
}


void tds_add_params(function_node_t * f, vector_t * params)
{
    c_assert(f && params && f->block && f->block->tds);

    size_t s = vector_size(params);
    size_t i;

    for(i = 0; i < s; ++i)
    {
	param_dec_t * p =
	    (param_dec_t *)vector_get_element_at(params, i);

	tds_add_entry(f->block->tds,
		      make_tds_entry(p->name_index,
				     p->type,
				     OBJ_PARAM));
    }
}


