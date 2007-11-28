
#include "anasyn.tab.h"
#include <stdlib.h>
#include "anasyn.h"

#include "lexique.h"

extern lexique_t * c_lexique;


int main()
{

    c_lexique = create_lexique();

    yyparse();

    free_lexique(c_lexique);

    return EXIT_SUCCESS;
}
