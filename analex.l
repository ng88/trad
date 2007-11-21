
%{

#include <stdio.h>
#include <stdlib.h>
#include  <string.h>

#include "anasyn.tab.h"

%}

entier  ([0-9]+)

%%

{entier}     { 
                yylval.vint = atoi(yytext);
                return T_CST_INT;
             }


\"[^"\n]*\"                           { 
                                        if(yytext[ yyleng - 2 ] == '\\' && yytext[ yyleng - 3 ] != '\\')
                                        {
                                            yyless(yyleng - 1);
                                            yymore();
                                        }
                                        else
                                        {
                                            yytext[ yyleng - 1 ] = 0;
                                            yylval.vstr = ++yytext;
                                            printf("CHAINE %s\n", yylval.vstr);
                                            return T_CST_STR;
                                        }
                                    }
                                    
\"[^"\n]*\n                         { 
                                        yytext[ yyleng - 1 ] = 0;
                                        
					printf("erreur chaine\n");
                                        
                                        return T_CST_STR;
                                    }


<*>.        { return yytext[0]; }
<*>\n       {  }




%%

