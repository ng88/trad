
#include "anasyn.tab.h"
#include <stdlib.h>
#include "anasyn.h"

#include "lexique.h"

#include "arbre_printer.h"


int main(int argc, char ** argv)
{
    yy_m_init();

    if(argc == 2)
	yyin = fopen(argv[1], "r");

    yyparse();

    print_tds(get_tds(), stdout, 0);

    fclose(yyin);

    yy_m_free();

    return EXIT_SUCCESS;
}
