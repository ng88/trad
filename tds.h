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
 */
typedef enum
{
    OBJ_CLASS,
    OBJ_PROC,
    OBJ_FUNC,
    OBJ_CTOR,
    OBJ_LOCAL_VAR,
    OBJ_FIELD,
} object_type_t;

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

#define tds_get_entry(tds, i) (tds_entry_t*)vector_get_element_at((tds)->entries, (i))
#define tds_count(tds) vector_size((tds)->entries)

tds_entry_t * tds_search_from_name(tds_t * tds, char * name);
tds_entry_t * tds_search_from_index(tds_t * tds, size_t index);

tds_entry_t * make_tds_entry(size_t name_index, var_type_t *t, object_type_t ot);

var_type_t * make_var_type(bool type_prim);
var_type_t * make_var_prim_type(primitive_type_t t);
var_type_t * make_var_user_type(size_t idf);


void free_tds(tds_t * t);
void free_tds_entry(tds_entry_t * e);
void free_var_type(var_type_t * t);

#endif
