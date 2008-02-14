#include "error.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

extern int yylineno;

void raise_error(error_t code, ...)
{
    int i;
    va_list(ap);

    va_start(ap, code);

    if(yylineno >= 0)
	fprintf(stderr, "erreur ligne %d : ", yylineno);
    else
	fputs("erreur : ", stderr);

    vfprintf(stderr, get_error_msg(code), ap);    
    fputs("\n", stderr);

    va_end(ap);

    exit(1);
 }


/* ca serait mieux avec un tableau... */
char * get_error_msg(error_t code)
{
    switch(code)
    {
    case ET_BAD_STRING: return "chaine de caractere incorrecte";
    case ET_IDF_TOO_LONG: return "identificateur trop long '%s'";
    case ET_SYNTAX_ERROR: return "erreur de syntaxe pres de '%s' (%s)";

    case ET_TYPE_MISMATCH: return "ne peut pas assigner un objet de type'%s' avec le type '%s'";
    case ET_CLASS_NOT_FOUND: return "classe non trouvee '%s'";
    case ET_FUNC_NOT_FOUND: return "aucune fonction/procedure nommee `%s' ne correspond au profil demande";
    case ET_CTOR_NOT_FOUND: return "constructeur non trouve";
    case ET_NO_CTOR_FOUND: return "la classe '%s' ne comporte aucun constructeur";
    case ET_CTOR_BAD_NAME: return "le constructeur doit porter le nom de la classe dans laquelle il est declare";

    case ET_MAIN_AMBIGUITY: return "une procedure main a deja ete declaree, il ne peut y avoir qu'un seul point d'entree";
    case ET_MAIN_NOT_FOUND: return "point d'entree non trouve, vous devez definir une procedure main()";

    case ET_CLASS_ALREADY_EXISTS: return "une classe nommee `%s' existe deja";
    case ET_FUNC_ALREADY_EXISTS: return "une fonction nommee `%s' existe deja avec ce profil";
    case ET_CTOR_ALREADY_EXISTS: return "un constructeur avec ce profil est deja defini pour `%s'";
    case ET_FIELD_ALREADY_EXISTS: return "un champs nomme `%s' existe deja";
    case ET_LVAR_ALREADY_EXISTS: return "une variable nommee `%s' existe deja";
    case ET_RETURN_IN_PROC: return "la procedure `%s' contient une instruction 'return', elle ne devrait pas";
    case ET_NO_RETURN_IN_FUNC: return "tous les chemins de la fonction `%s' ne comportent pas de 'return'";

    case ET_VAR_NO_DEC: return "la variable `%s' n'est pas declaree";

    case ET_TYPE_BIN_ERR: return "type invalide, ne peut effectuer l'operation `%s' %s `%s'";
    case ET_TYPE_UNA_ERR: return "type invalide, ne peut effectuer l'operation %s `%s'";
    case ET_TYPE_MBR_ERR: return "l'operateur '.' ne peut pas s'utiliser sur un type primitif `%s', mais seulement sur un objet.";
    }

    return "unknow";
}
