
#include "anasyn.tab.h"
#include <stdlib.h>
#include "anasyn.h"

#include "lexique.h"


int main(int argc, char ** argv)
{
    yy_m_init();

    if(argc == 2)
	yyin = fopen(argv[1], "r");

    yyparse();

    fclose(yyin);

    yy_m_free();

    return EXIT_SUCCESS;
}
