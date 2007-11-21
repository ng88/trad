

%{


#include <stdio.h>
#include <stdlib.h>
#include <string.h>



%}

%union
{
    int vint;
    char * vstr;
}

%start prog

%token T_CST_INT
%token T_CST_STR



%%

prog:
      T_CST_STR | T_CST_INT;


%%


int main()
{

    yyparse();


    return EXIT_SUCCESS;
}
