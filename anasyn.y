

%{


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "anasyn.h"

#include "arbre_printer.h"
#include "assert.h"
#include "lexique.h"

#define MAIN_NAME 1

/* contient la pile des blocs ouverts */
stack_t * block_stack = NULL;
class_node_t * _current_class = NULL;
 function_node_t * _main = NULL;
/* tds de base */
tds_t * base_tds = NULL;


bloc_instr_node_t * current_block();
tds_t * last_tds();
class_node_t * current_class();

%}



%start programme

%token <vint> T_CST_INT
%token <vdouble> T_CST_DBL
%token <index_lexique> T_CST_STR
%token <index_lexique> T_IDF

%token FIN_FICHIER
%token MC_CLASS
%token MC_END
%token MC_INHERIT
%token MC_PRIVATE
%token MC_PUBLIC
%token MC_REAL
%token MC_INTEGER
%token MC_STRING
%token MC_VOID
%token MC_SUPER
%token MC_RETURN
%token MC_NEW
%token MC_WHILE
%token MC_DO
%token MC_IF
%token MC_THEN
%token MC_ELSE
%token MC_ENDIF
%token MC_VAR

%token OP_UNARY_MINUS
%token OP_EQ
%token OP_NE
%token OP_LE
%token OP_GE
%token OP_LT
%token OP_GT
%token OP_AND
%token OP_OR
%token OP_PLUS
%token OP_MINUS
%token OP_DIV
%token OP_MUL
%token OP_AFFECT
%token OP_BRACKET_O
%token OP_BRACKET_C
%token OP_BRACE_O
%token OP_BRACE_C
%token OP_SEMICOLON
%token OP_COMMA
%token OP_MEMBER

%type <expr> exp
%type <params> param_eff
%type <params> param_eff_non_vide
%type <rval> rvalue
%type <instr> instruction
%type <instr> boucle
%type <instr> cond
%type <instr> affectation
%type <bloc> bloc_inst
%type <bloc> else
%type <bloc> liste_instruction_non_vide
%type <bloc> liste_instruction
%type <bloc> bloc_inst2
%type <direct_call> appel_membre
%type <call> appel
%type <idf_list> liste_idf
%type <type> type
%type <idf_list_type> d_var_class
%type <idf_list_type> d_var
%type <tds> liste_var_non_vide
%type <mclass> class
%type <mfn> d_construct
%type <mfn> d_fonction
%type <mfn> d_procedure
%type <scope> etat
%type <type_list> param
%type <type_list> liste_type_idf

/* Priorité des opérateurs */

%left OP_AND OP_OR

%nonassoc OP_EQ OP_NE OP_LE OP_GE OP_LT OP_GT

%left OP_PLUS OP_MINUS
%left OP_DIV OP_MUL

%nonassoc OP_UNARY_MINUS

%%


type:
      MC_INTEGER { $$ = make_var_prim_type(PT_INT); }
    | MC_STRING  { $$ = make_var_prim_type(PT_STRING); }
    | MC_REAL    { $$ = make_var_prim_type(PT_REAL); }
    | T_IDF      { $$ = make_var_user_type(get_tds(), $1); }
    ;

etat:
       MC_PRIVATE { $$ = ST_PRIVATE; }
     | MC_PUBLIC  { $$ = ST_PUBLIC; }
   ;

exp:
     // T_IDF inclu dans appel
     appel                                            { $$ = make_expr_from_call_expr_node($1); }
   | T_CST_INT                                        { $$ = make_constant_int_expr_node($1); }
   | T_CST_STR                                        { $$ = make_constant_str_expr_node($1); }
   | T_CST_DBL                                        { $$ = make_constant_dbl_expr_node($1); }
   | OP_BRACKET_O exp OP_BRACKET_C                    { $$ = $2; }
   | OP_MINUS exp %prec OP_UNARY_MINUS                { $$ = make_unary_expr_node(UNT_MINUS, $2); }
   | exp OP_DIV exp                                   { $$ = make_binary_expr_node(BNT_DIV, $1, $3); }
   | exp OP_MUL exp                                   { $$ = make_binary_expr_node(BNT_MUL, $1, $3); }
   | exp OP_PLUS exp                                  { $$ = make_binary_expr_node(BNT_PLUS, $1, $3); }
   | exp OP_MINUS exp                                 { $$ = make_binary_expr_node(BNT_MINUS, $1, $3); }
   | exp OP_EQ exp                                    { $$ = make_binary_expr_node(BNT_EQ, $1, $3); }
   | exp OP_NE exp                                    { $$ = make_binary_expr_node(BNT_NE, $1, $3); }
   | exp OP_LE exp                                    { $$ = make_binary_expr_node(BNT_LE, $1, $3); }
   | exp OP_GE exp                                    { $$ = make_binary_expr_node(BNT_GE, $1, $3); }
   | exp OP_LT exp                                    { $$ = make_binary_expr_node(BNT_LT, $1, $3); }
   | exp OP_GT exp                                    { $$ = make_binary_expr_node(BNT_GT, $1, $3); }
   | exp OP_AND exp                                   { $$ = make_binary_expr_node(BNT_AND, $1, $3); }
   | exp OP_OR exp                                    { $$ = make_binary_expr_node(BNT_OR, $1, $3); }
   ;

appel:
     appel_membre                   { $$ = make_call_from_direct_call_expr_node($1); }
   | appel OP_MEMBER appel_membre   { $$ = make_call_from_member_expr_node($1, $3);  }
   ;

appel_membre:
     T_IDF OP_BRACKET_O param_eff OP_BRACKET_C { $$ = make_fn_direct_call_expr_node($1, $3); }
   | T_IDF                                     { $$ = make_idf_direct_call_expr_node($1); }
   ;

param_eff_non_vide:
     exp
     {
	 $$ = make_param_eff_expr_node();
	 add_param_eff($$, $1);
     }
   | param_eff OP_COMMA exp
     {
	 $$ = $1;
	 add_param_eff($$, $3);
     }
   ;

param_eff:
     param_eff_non_vide  { $$ = $1; }
   | /* vide */          { $$ = make_param_eff_expr_node(); }
   ;

instruction:
     MC_SUPER OP_BRACKET_O param_eff OP_BRACKET_C OP_SEMICOLON { $$ = make_super_instr_node($3); }
   | appel OP_SEMICOLON                                        { $$ = make_call_instr_node($1); }
   | affectation OP_SEMICOLON                                  { $$ = $1; }
   | boucle                                                    { $$ = $1; }
   | cond                                                      { $$ = $1; }
   | MC_RETURN OP_BRACKET_O exp OP_BRACKET_C OP_SEMICOLON      { $$ = make_return_instr_node($3); }
   ;

affectation:
     T_IDF OP_AFFECT rvalue { $$ = make_affect_instr_node($1, $3); }
   ;

rvalue:
     exp
     {
	 $$ = make_rvalue_expr_node($1);
     }
   | MC_NEW T_IDF OP_BRACKET_O param_eff OP_BRACKET_C
     {
	 $$ = make_rvalue_new_node($2, $4);
     }
   ;

d_var:
     MC_VAR type liste_idf OP_SEMICOLON
     {
	 $$.idf_list = $3;
	 $$.type = $2;
     }
   ;

liste_idf: 
     T_IDF
     {
	 $$ = create_vector(2);
	 vector_add_element($$, BOX_UINT($1));
     }
   | liste_idf OP_COMMA T_IDF 
     {
	 $$ = $1;
	 vector_add_element($$, BOX_UINT($3));
     }
   ;

liste_var_non_vide:
     d_var
     {
	 $$ = current_block()->tds;
	 tds_add_entries($$, $1.idf_list, $1.type, OBJ_LOCAL_VAR);
	 free_vector($1.idf_list, 0);
     }
   | liste_var_non_vide d_var
     {
	 $$ = $1;
	 tds_add_entries($$, $2.idf_list, $2.type, OBJ_LOCAL_VAR);
	 free_vector($2.idf_list, 0);
     }
   ;

liste_instruction_non_vide:
     instruction
     {
	 $$ = current_block();
	 add_instr_bloc($$, $1);
     }
   | liste_instruction_non_vide instruction
     {
	 add_instr_bloc($1, $2);
     }
   ;

liste_instruction:
     liste_instruction_non_vide   { $$ = $1; }
   | /* vide */                   { $$ = current_block(); }
   ;

bloc_inst:
   { /*  regle permettant simplement la creation, l'empilement et le depilemement du bloc courant . */

       bloc_instr_node_t * n = make_bloc_instr_node();
       n->tds = make_tds(last_tds());

       stack_push(block_stack, n);
   }
   bloc_inst2
   {
       $$ = stack_pop(block_stack);
   };

bloc_inst2:
       instruction
       {
	   $$ = current_block();
	   add_instr_bloc($$, $1);
       }
     | OP_BRACE_O liste_var_non_vide liste_instruction OP_BRACE_C
       {
	   $$ = $3;
	   $$->tds = $2;
       }
     | OP_BRACE_O liste_instruction OP_BRACE_C
       {
	   /* pas de tds donc pas dans la pile des blocks */ 
	   $$ = $2;
       }
     ;

boucle:
       MC_WHILE exp MC_DO bloc_inst     { $$ = make_loop_instr_node($2, $4); }
     ;

cond:
       MC_IF exp MC_THEN bloc_inst else MC_ENDIF   { $$ = make_cond_instr_node($2, $4, $5); }
     ;

else:
       MC_ELSE bloc_inst     { $$ = $2; }
     | /* vide */            { $$ = NULL; }
     ;

d_var_class:
      etat type liste_idf OP_SEMICOLON
     {
	 $$.idf_list = $3;
	 $$.type = $2;
	 
	 tds_add_field_entries(get_tds(), $3, $2, $1);
     }
     ;

d_construct:
      etat T_IDF param bloc_inst
    {
	if($2 != CTOR_NAME)
	    raise_error(ET_CTOR_BAD_NAME);

	$$ = make_constructor_node($1, $3, $4);

	tds_add_entry(current_class()->tds,
		      make_tds_constructor_entry($$));

	tds_add_params($$, $3);
    }
    ;

d_fonction:
      etat type T_IDF param bloc_inst
    {
	$$ = make_function_node($3, $1, $4, $5);
	$$->ret_type = $2;

	tds_add_entry(current_class()->tds,
		      make_tds_function_entry($$));

	tds_add_params($$, $4);
    }
    ;

d_procedure:
      etat MC_VOID T_IDF param bloc_inst
    {
	$$ = make_procedure_node($3, $1, $4, $5);

	tds_add_entry(current_class()->tds,
	      make_tds_procedure_entry($$));

	tds_add_params($$, $4);

	if($3 == MAIN_NAME && vector_size($4) == 0)
	{
	    if(_main)
		raise_error(ET_MAIN_AMBIGUITY);
	    else
		_main = $$;
	}

    }
    ;

param:
      OP_BRACKET_O liste_type_idf OP_BRACKET_C
      {
	  $$ = $2;
      }
    | OP_BRACKET_O OP_BRACKET_C
      {
	  $$ = create_vector(1);
      }
    ;

liste_type_idf: 
      type T_IDF
      {
	  $$ = create_vector(2);
	  vector_add_element($$, make_param_dec($2, $1));
      }
    | liste_type_idf OP_COMMA type T_IDF
      {
	  $$ = $1;
	  vector_add_element($$, make_param_dec($4, $3));
      }
    ;

declaration:
      d_var_class
    | d_construct
    | d_fonction
    | d_procedure
    ;

liste_declaration_non_vide:
      declaration
    | liste_declaration_non_vide declaration
    ;

liste_declaration:
       liste_declaration_non_vide
     | /* vide */
     ;

class:
        MC_CLASS T_IDF
        {
	    _current_class = make_class_node($2, get_tds());
	    tds_add_entry(get_tds(),
			  make_tds_class_entry(_current_class));
        }
        liste_declaration MC_END { $$ = current_class(); }

     |  MC_CLASS T_IDF MC_INHERIT T_IDF
        {
	    _current_class = make_class_node($2, get_tds());
	    tds_add_entry(get_tds(),
			  make_tds_class_entry(_current_class));

	    _current_class->super = resolve_class_identifier(get_tds(), $4);

	}
        liste_declaration MC_END { $$ = current_class(); }
     ;

l_class:
	l_class class
     |  class
     ;

programme:
	l_class FIN_FICHIER                                         {printf("programme -> l_class FIN_FICHIER\n");return 0;}
     ;

%%

bloc_instr_node_t * current_block()
{
    c_assert(block_stack && !stack_empty(block_stack));

    c_warning(stack_top(block_stack));

    return (bloc_instr_node_t *)stack_top(block_stack);
}

tds_t * last_tds()
{
    c_assert(block_stack);


    stack_node_t * ret = block_stack->top;

    while(ret)
    {
	bloc_instr_node_t * block =
	    (bloc_instr_node_t *)ret->val;

	if(block && block->tds)
	    return block->tds;

	ret = ret->prev;
    }

    return get_tds();
}

class_node_t * current_class()
{
    c_assert(_current_class);
    return _current_class;
}

tds_t * get_tds()
{
    c_assert(base_tds);
    return base_tds;
}

function_node_t * get_main_function()
{
    return _main;
}


void yy_m_init()
{
    c_lexique = create_lexique();
    /* used for printing constructor name */
    lexique_add(c_lexique, strdup("constructor"));
    lexique_add(c_lexique, strdup("main"));

    block_stack = create_stack();
    base_tds = make_tds(NULL);
}

void yy_m_free()
{
    free_tds(base_tds);
    stack_free(block_stack, 0);
    free_lexique(c_lexique);
}

