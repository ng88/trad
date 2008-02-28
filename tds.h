#ifndef TDS_H
#define TDS_H

#include "lexique.h"
#include "vector.h"
#include "bool.h"

/** Declaration de la table des symboles
    On en a une par bloc.
*/

/** Types primitifs
 */
typedef enum
{
    PT_INT = 0,
    PT_REAL = 1,
    PT_UNKNOW = 2,
    PT_STRING = 3,
    PT_ANY = 4, /* any object */
    PT_NIL = 5, /* nil type */
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

#define OBJ_FNP (OBJ_PROC | OBJ_FUNC | OBJ_CTOR)

#define OBJ_VAR (OBJ_LOCAL_VAR | OBJ_PARAM | OBJ_FIELD)

#define OBJ_NO_LOCAL (OBJ_PROC | OBJ_FUNC | OBJ_CTOR | OBJ_FIELD)

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

/** attention double evaluation de t */
#define TYPE_IS_UNKNOWN(t)  ((t)->type_prim && (t)->type.prim == PT_UNKNOW)
#define TYPE_IS_ANY(t)  ((t)->type_prim && (t)->type.prim == PT_ANY)
#define TYPE_IS_NIL(t)  ((t)->type_prim && (t)->type.prim == PT_NIL)
#define TYPE_SET_UNKNOWN(t) do { (t)->type_prim = true;(t)->type.prim = PT_UNKNOW; } while(0);

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

    /** export C ? */
    bool cexport;

    /** Informations spécifiques, en fonction du type d'objet */
    union
    {
	struct _function_node_t * fn;
	struct _class_node_t *    cl;	
	struct _field_node_t *    fl;
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
void tds_add_field_entries(tds_t * tds, vector_t * indices, var_type_t *t, scope_t s, struct _class_node_t * parent);

#define tds_get_entry(tds, i) ((tds_entry_t*)vector_get_element_at((tds)->entries, (i)))
#define tds_count(tds) (vector_size((tds)->entries))

/** Recherche depuis le nom, ot_mask est un masque indiquant le type des objets a chercher.
 rec indique si c'est recursif ou pas. */
tds_entry_t * tds_search_from_name(tds_t * tds, char * name, object_type_t ot_mask, bool rec);

/** Recherche directe depuis l'index dans le lexique. */
tds_entry_t * tds_search_from_index(tds_t * tds, size_t index, object_type_t ot_mask, bool rec);

/** Recherche une fonction du nom index et de type de parametres params. 
/!\ Attention, params est un vecteur de var_type_t * si v_mode == true
                  ou un vecteur de param_dec_t * si v_mode == false
*/
tds_entry_t * tds_search_function(tds_t * tds, size_t index,  vector_t * params, bool rec, bool v_mode);

/** Verifie que le type des parametres de v correspond aux types
 attendus pour fn

/!\ Attention, v est un vecteur de var_type_t * si v_mode == true
                  ou un vecteur de param_dec_t * si v_mode == false

*/
bool param_equals(struct _function_node_t * fn, vector_t * v, bool v_mode);

/** Indique si 2 types sont strictement les mm. */
bool var_type_equals(var_type_t * v1, var_type_t * v2);

/** Indique si v2 peut etre assigne a du v1. */
bool can_assign_var_type(var_type_t * v1, var_type_t * v2);

struct _class_node_t * resolve_class_identifier(tds_t * tds, size_t name_index);

tds_entry_t * make_tds_class_entry(struct _class_node_t * cl);
tds_entry_t * make_tds_function_entry(struct _function_node_t * fn);
tds_entry_t * make_tds_procedure_entry(struct _function_node_t * fn);
tds_entry_t * make_tds_constructor_entry(struct _function_node_t * fn);
tds_entry_t * make_tds_field_entry(size_t name_index, var_type_t *t, scope_t s, struct _class_node_t * parent);
tds_entry_t * make_tds_entry(size_t name_index, var_type_t *t, object_type_t ot);

void tds_add_params(struct _function_node_t * f, vector_t * params);

var_type_t * make_var_type(bool type_prim);
var_type_t * make_var_prim_type(primitive_type_t t);
var_type_t * make_var_user_type(tds_t * tds, size_t idf);

var_type_t * copy_var_type(var_type_t * n);

struct _class_node_t * entry_get_class(tds_entry_t * e);
struct _function_node_t * entry_get_function(tds_entry_t * e);

param_dec_t * make_param_dec(size_t name, var_type_t * t);
void free_param_dec(param_dec_t * p);

char * get_var_type(var_type_t * t);


void free_tds(tds_t * t);
void free_tds_entry(tds_entry_t * e);
void free_var_type(var_type_t * t);


/** Retourne vrai si la fonction fn est surchargee  */
bool is_an_overloaded_function(struct _function_node_t * fn);

/** Recupere la derniere surcharge de fn a partir de cl*/
struct _function_node_t * get_last_overload(struct _function_node_t * fn, struct _class_node_t * cl);

/** Retourne le ctor par defaut (sans param) ou NULL */
struct _function_node_t * get_default_ctor(struct _class_node_t * cl);

/** Indique si a herite de b */
bool inherits_from(struct _class_node_t * a, struct _class_node_t * b);


#endif
