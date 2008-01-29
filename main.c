
#include "anasyn.tab.h"
#include <stdlib.h>
#include "anasyn.h"

#include "lexique.h"


int main(int argc, char ** argv)
{
    yyinit();

    if(argc == 2)
	yyin = fopen(argv[1], "r");

    yyparse();

    fclose(yyin);

    yyfree();

    return EXIT_SUCCESS;
}
