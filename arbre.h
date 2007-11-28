
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


/** Noeud */
typedef struct _node_t
{
    node_type_t type;
    union
    {
	struct _bin_node_t * bin;
	struct _una_node_t * una;
    } node;
} node_t;


/** Noeud binaire */
struct _bin_node_t
{
    bin_node_type_t type;
    node_t * gauche;
    node_t * droit;
} bin_node_t;


/** Noeud unaire */
struct _una_node_t
{
    una_node_type_t type;
    node_t * fils;
} una_node_t;


/** Noeud constante */
struct _cst_node_t
{
    cst_node_type_t type;
    union
    {
	char * vstr;
	char * vidf;
	int    vint;
    } val;
} cst_node_t;


/** Construit un noeud de base, utilis� par les autres 
 *  fonctions de cr�ations, pas directement.
 */
node_t * make_node();

/** Construit un noeud binaire
 */
node_t * make_binary_node(bin_node_type_t t, node_t g, node_t d);



#endif