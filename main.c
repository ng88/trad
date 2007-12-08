
#include "anasyn.tab.h"
#include <stdlib.h>
#include "anasyn.h"

#include "lexique.h"

extern lexique_t * c_lexique;


int main(int argc, char ** argv)
{

    c_lexique = create_lexique();

    if(argc == 2)
	yyin = fopen(argv[1], "r");

    yyparse();

    fclose(yyin);

    free_lexique(c_lexique);

    return EXIT_SUCCESS;
}
