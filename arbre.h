
#ifndef ARBRE_H
#define ARBRE_H


/** Types de noeuds */
typedef enum
{
    NT_BINARY,
    NT_UNARY,
    NT_CONST,
} node_type_t;


/** Types de noeuds binaires */
typedef enum
{
    BNT_MUL,
    BNT_DIV,
    BNT_PLUS,
    BNT_MINUS,
    BNT_EQ,
    BNT_NE,
    BNT_LE,
    BNT_LT,
    BNT_GT,
    BNT_AND,
    BNT_OR,
} bin_node_type_t;

/** Types de noeuds constantes */
typedef enum
{
    CNT_INT,
    CNT_STR,
    CNT_IDF,
} cst_node_type_t;


/** Types de noeuds unaires */
typedef enum
{
    UNT_MINUS,

} una_node_type_t;


struct _bin_node_t;
struct _una_node_t;
struct _cst_node_t;

/** Noeud */
typedef struct _node_t
{
    node_type_t type;
    union
    {
	struct _bin_node_t * bin;
	struct _una_node_t * una;
	struct _cst_node_t * cst;
    } node;
} node_t;


/** Noeud binaire */
typedef struct _bin_node_t
{
    bin_node_type_t type;
    node_t * gauche;
    node_t * droit;
} bin_node_t;


/** Noeud unaire */
typedef struct _una_node_t
{
    una_node_type_t type;
    node_t * fils;
} una_node_t;


/** Noeud constante */
typedef struct _cst_node_t
{
    cst_node_type_t type;
    union
    {
	char * vstr;
	char * vidf;
	int    vint;
    } val;
} cst_node_t;


/** Construit un noeud de base, utilisé par les autres 
 *  fonctions de créations, pas directement.
 */
node_t * make_node(node_type_t t);

node_t * make_binary_node(bin_node_type_t t, node_t * g, node_t * d);
node_t * make_unary_node(una_node_type_t t, node_t * f);
node_t * make_constant_node(cst_node_type_t t);


/** Libère n'importe quel noeud */
void free_node(node_t * n);

/** Fonctions de nettoyage appelées par free_node() */
void free_binary_node(bin_node_t * n);
void free_unary_node(una_node_t * n);
void free_constant_node(cst_node_t * n);

#endif
