
#ifndef ARBRE_EXPR_H
#define ARBRE_EXPR_H

#include <stdio.h>
#include "vector/vector.h"

/** pour l'instant, mais ca devrait changer quand on fera
   les tdf...*/
typedef char* idf_t;

/** Declarations pour les arbres d'expressions
 */

/** Types de noeuds */
typedef enum
{
    NT_BINARY,
    NT_UNARY,
    NT_CONST,
    NT_CALL,
} expr_node_type_t;


/** Types de noeuds binaires 
 */
typedef enum
{
    BNT_MUL,
    BNT_DIV,
    BNT_PLUS,
    BNT_MINUS,
    BNT_EQ,
    BNT_NE,
    BNT_LE,
    BNT_GE,
    BNT_LT,
    BNT_GT,
    BNT_AND,
    BNT_OR,
} bin_expr_node_type_t;

/** Types de noeuds constantes */
typedef enum
{
    CNT_INT,
    CNT_STR,
    CNT_IDF,
    CNT_DBL,
} cst_expr_node_type_t;


/** Types de noeuds unaires */
typedef enum
{
    UNT_MINUS,

} una_expr_node_type_t;

/** Types des rvalues */
typedef enum
{
    RNT_NEW,
    RNT_EXPR,
} rvalue_node_type_t;



struct _bin_expr_node_t;
struct _una_expr_node_t;
struct _cst_expr_node_t;
struct _call_expr_node_t;

/** Noeud */
typedef struct _expr_node_t
{
    expr_node_type_t type;
    union
    {
	struct _bin_expr_node_t * bin;
	struct _una_expr_node_t * una;
	struct _cst_expr_node_t * cst;
	struct _call_expr_node_t * call;
    } node;
} expr_node_t;


/** Noeud binaire */
typedef struct _bin_expr_node_t
{
    bin_expr_node_type_t type;
    expr_node_t * gauche;
    expr_node_t * droit;
} bin_expr_node_t;


/** Noeud unaire */
typedef struct _una_expr_node_t
{
    una_expr_node_type_t type;
    expr_node_t * fils;
} una_expr_node_t;


/** Noeud constante */
typedef struct _cst_expr_node_t
{
    cst_expr_node_type_t type;
    union
    {
	size_t index_str;
	idf_t  vidf;
	int    vint;
	double vdouble;
    } val;
} cst_expr_node_t;


/** Parametres effectifs */
typedef struct _param_eff_expr_node_t
{
    /* on utilise un tableau,
     c'est mieux pour les parametres*/
    vector_t * params;
} param_eff_expr_node_t;


/** Noeud d'appel */
typedef struct _call_expr_node_t
{
} call_expr_node_t;



/** Noeud new */
typedef struct _new_expr_node_t
{
    idf_t idf;
    param_eff_expr_node_t * params;
} new_expr_node_t;



/** Noeud rvalue */
typedef struct _rvalue_node_t
{
    rvalue_node_type_t type;
    union
    {
	new_expr_node_t * nnew;
	expr_node_t * expr;
    } node;
} rvalue_node_t;


/** Construit un noeud de base, utilisé par les autres 
 *  fonctions de créations, pas directement.
 */
expr_node_t * make_expr_node(expr_node_type_t t);

expr_node_t * make_binary_expr_node(bin_expr_node_type_t t, expr_node_t * g, expr_node_t * d);
expr_node_t * make_unary_expr_node(una_expr_node_type_t t, expr_node_t * f);
expr_node_t * make_call_expr_node();
expr_node_t * make_constant_expr_node(cst_expr_node_type_t t);
expr_node_t * make_constant_str_expr_node(size_t index_str);
expr_node_t * make_constant_idf_expr_node(idf_t vidf);
expr_node_t * make_constant_int_expr_node(int vint);
expr_node_t * make_constant_dbl_expr_node(double vd);

param_eff_expr_node_t * make_param_eff_expr_node();
new_expr_node_t *  make_new_expr_node(idf_t idf, param_eff_expr_node_t * p);
rvalue_node_t * make_rvalue_node(rvalue_node_type_t t);
rvalue_node_t * make_rvalue_expr_node(expr_node_t * n);
rvalue_node_t * make_rvalue_new_node(idf_t idf, param_eff_expr_node_t * p);

/** Ajout le parametre effectif e au noeud p
 */
void add_param_eff(param_eff_expr_node_t * p, expr_node_t * e);
size_t param_eff_count(param_eff_expr_node_t * p);
expr_node_t * param_eff_get(param_eff_expr_node_t * p, size_t index);

/** Affiche un noeud d'expression */
void print_expr_node(expr_node_t * e, FILE * f);

void print_binary_expr_node(bin_expr_node_t * e, FILE * f);
void print_unary_expr_node(una_expr_node_t * e, FILE * f);
void print_constant_expr_node(cst_expr_node_t * e, FILE * f);

void print_param_eff_expr_node(param_eff_expr_node_t * n, FILE * f);
void print_new_expr_node(new_expr_node_t * n, FILE * f);
void print_rvalue_node(rvalue_node_t * n, FILE * f);

void print_call_expr_node(call_expr_node_t * c, FILE * f);

/** Libère n'importe quel noeud d'expression */
void free_expr_node(expr_node_t * n);

/** Fonctions de nettoyage appelées par free_node() */
void free_binary_expr_node(bin_expr_node_t * n);
void free_unary_expr_node(una_expr_node_t * n);
void free_constant_expr_node(cst_expr_node_t * n);

void free_param_eff_expr_node(param_eff_expr_node_t * n);
void free_new_expr_node(new_expr_node_t * n);
void free_rvalue_node(rvalue_node_t * n);

void free_call_expr_node(call_expr_node_t * c);

#endif
