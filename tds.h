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
    OBJ_LOCAL_VAR,
} object_type_t;


typedef char class_node_t; // en attendant...

/** Type d'une variable 
 */
typedef struct
{
    union
    {
	primitive_type_t prim;
	class_node_t * uclass;
    } type;

    /** Est-ce un type primitif ?*/
    bool type_prim;
} var_type_t;

/** Infos sur les fonctions
 */
typedef struct
{
    vector_t * params;
    var_type_t * return_type;
    class_node_t * cl;
} function_infos_t;

/** Infos sur les procedures
 */
typedef struct
{
    vector_t * params;
    class_node_t * cl;
} proc_infos_t;

/** Infos sur les classes
 */
typedef struct
{
} class_infos_t;



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
	function_infos_t * fn;
	proc_infos_t *     pr;
	class_infos_t *    cl;
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
#define tds_get_entry(tds, i) (tds_entry_t*)vector_get_element_at((tds)->entries, (i))
tds_entry_t * tds_search_from_name(tds_t * tds, char * name);
tds_entry_t * tds_search_from_index(tds_t * tds, size_t index);

tds_entry_t * make_tds_entry(size_t name_index, var_type_t *t, object_type_t ot);

function_infos_t * make_function_infos(class_node_t * cl);
proc_infos_t * make_proc_infos(class_node_t * cl);
class_infos_t * make_class_infos();
var_type_t * make_var_type(bool type_prim);



void free_tds(tds_t * t);
void free_tds_entry(tds_entry_t * e);
void free_function_infos(function_infos_t *e);
void free_proc_infos(proc_infos_t *e);
void free_class_infos(class_infos_t * e);
void free_var_type(var_type_t * t);

#endif
