#include "error.h"
#include <stdarg.h>


void raise_error(size_t line, error_t code, ...)
{
    int i;
    va_list(ap);

    va_start(ap, code);

    fprintf(f, "erreur sur la ligne %u : ");
    vfprintf(stderr, get_error_msg(code), ap);    
    fputs("\n", stderr);

    va_end(ap);

    exit(1);
 }



char * get_error_msg(error_t code)
{
    switch(code)
    {
    case ET_BAD_STRING: return "chaine de caractere incorrecte";
    case ET_IDF_TOO_LONG: return "identificateur trop long";
    case ET_SYNTAX_ERROR: return "erreur de syntaxe pres de '%s' (%s)";

    case ET_TYPE_MISMATCH: return "ne peut pas assigner un objet de type'%s' avec le type '%s'";
    case ET_CLASS_NOT_FOUND: return "classe non trouvee '%s'";
    case ET_FUNC_NOT_FOUND: return "fonction non trouvee";
    case ET_CTOR_NOT_FOUND: return "constructeur non trouve";
    case ET_NO_CTOR_FOUND: return "la classe '%s' ne comporte aucun constructeur";
    }
}
