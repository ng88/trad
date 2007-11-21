

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

%token mc_class
%token mc_end
%token mc_inherit
%token mc_private
%token mc_public
%token mc_integer
%token idf

%%

prog:
      T_CST_STR | T_CST_INT;


%%


