#ifndef TDS_H
#define TDS_H

#include "lexique.h"
#include "vector/vector.h"
#include "bool.h"

/** Declaration de la table des symboles
    On en a une par bloc.
*/

/** Types primitifs
 */
typedef enum
{
    PT_STRING,
    PT_INT,
    PT_REAL,
} primitive_type_t;

/** Types d'objet 
 *  (en puissance de 2 pour faire des masques facilement)
 */
typedef enum
{
    OBJ_CLASS = 1,
    OBJ_PROC = 2,
    OBJ_FUNC = 4,
    OBJ_CTOR = 8,
    OBJ_LOCAL_VAR = 16,
    OBJ_PARAM = 32,
    OBJ_FIELD = 64,
} object_type_t;

#define OBJ_NONE 0
#define OBJ_ALL (OBJ_CLASS | OBJ_PROC | OBJ_FUNC | \
                 OBJ_CTOR | OBJ_LOCAL_VAR | \
		 OBJ_FIELD | OBJ_PARAM)

/** Portee
 */
typedef enum
{
    ST_PRIVATE = 0,
    ST_PUBLIC,
} scope_t;


struct _class_node_t;
struct _function_node_t;


/** Type d'une variable 
 */
typedef struct
{
    union
    {
	primitive_type_t prim;
	struct _class_node_t * uclass;
    } type;

    /** Est-ce un type primitif ?*/
    bool type_prim;

    /** Nombre de reference, pour la destruction*/
    unsigned char ref;
} var_type_t;

typedef struct
{
    size_t name_index;
    var_type_t * type;
} param_dec_t;


/** Une entree dans la TDS
*/
typedef struct _tds_entry_t
{
    /** Index du nom dans le lexique */
    size_t name_index;

    /** Type */
    var_type_t * type;

    /** Type de l'objet */
    object_type_t otype;

    /** Informations spécifiques, en fonction du type d'objet */
    union
    {
	struct _function_node_t * fn;
	struct _class_node_t *    cl;
	scope_t                   field_scope;
    } infos;

} tds_entry_t;

/** TDS
 */
typedef struct _tds_t
{

    struct _tds_t * parent;

    /* On suppose qu'il n'y aura pas des 
       milliers d'entrées pour un bloc
     */
    vector_t * entries;

} tds_t;





tds_t * make_tds(tds_t * parent);

void tds_add_entry(tds_t * tds, tds_entry_t * e);
/** Ajout multiple*/
void tds_add_entries(tds_t * tds,vector_t * indices, var_type_t *t,object_type_t ot);
void tds_add_field_entries(tds_t * tds,vector_t * indices, var_type_t *t,scope_t s);

#define tds_get_entry(tds, i) ((tds_entry_t*)vector_get_element_at((tds)->entries, (i)))
#define tds_count(tds) vector_size((tds)->entries)


tds_entry_t * tds_search_from_name(tds_t * tds, char * name, object_type_t ot_mask);
tds_entry_t * tds_search_from_index(tds_t * tds, size_t index, object_type_t ot_mask);


struct _class_node_t * resolve_class_identifier(tds_t * tds, size_t name_index);

tds_entry_t * make_tds_class_entry(struct _class_node_t * cl);
tds_entry_t * make_tds_function_entry(struct _function_node_t * fn);
tds_entry_t * make_tds_procedure_entry(struct _function_node_t * fn);
tds_entry_t * make_tds_constructor_entry(struct _function_node_t * fn);
tds_entry_t * make_tds_field_entry(size_t name_index, var_type_t *t, scope_t s);
tds_entry_t * make_tds_entry(size_t name_index, var_type_t *t, object_type_t ot);

void tds_add_params(struct _function_node_t * f, vector_t * params);

var_type_t * make_var_type(bool type_prim);
var_type_t * make_var_prim_type(primitive_type_t t);
var_type_t * make_var_user_type(tds_t * tds, size_t idf);

struct _class_node_t * entry_get_class(tds_entry_t * e);
struct _function_node_t * entry_get_function(tds_entry_t * e);

param_dec_t * make_param_dec(size_t name, var_type_t * t);
void free_param_dec(param_dec_t * p);

char * get_var_type(var_type_t * t);


void free_tds(tds_t * t);
void free_tds_entry(tds_entry_t * e);
void free_var_type(var_type_t * t);

#endif
