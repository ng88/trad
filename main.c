
#include "anasyn.tab.h"
#include <stdlib.h>
#include "anasyn.h"

#include "lexique.h"

#include "arbre_printer.h"
#include "arbre_compile.h"
#include "arbre_sem.h"

int main(int argc, char ** argv)
{
    yy_m_init();

    char * dest = NULL;

    if(argc >= 2)
	yyin = fopen(argv[1], "r");

    if(argc >= 3)
	dest = argv[2];

    yyparse();

    print_tds(get_tds(), stdout, 0);

    if(dest)
    {
	resolve_start(get_tds());

	compile_env_t * e = make_compile_env(dest);

	compile_start(e, get_tds(), get_main_function());

	free_compile_env(e);
    }

    fclose(yyin);

    yy_m_free();

    return EXIT_SUCCESS;
}
