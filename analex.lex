
%{

#include <stdio.h>
#include <stdlib.h>
#include  <string.h>

#include "anasyn.tab.h"

%}

entier  ([0-9]+)
idf     ([a-zA-Z][a-zA-Z0-9]*)

%x COMMENT_MODE

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


\/\*                             { BEGIN(COMMENT_MODE); }

<COMMENT_MODE>\*\/     { BEGIN(INITIAL); }
<COMMENT_MODE>.        {  }
<COMMENT_MODE>\n       {  }

class      {  return MC_CLASS; }
end        {  return MC_END; }
inherit    {  return MC_INHERIT; }
private    {  return MC_PRIVATE; }
public     {  return MC_PUBLIC; }
integer    {  return MC_INTEGER; }
string     {  return MC_STRING; }
super      {  return MC_SUPER; }
return     {  return MC_RETURN; }
new        {  return MC_NEW; }
while      {  return MC_WHILE; }
do         {  return MC_DO; }
if         {  return MC_IF; }
then       {  return MC_THEN; }
else       {  return MC_ELSE; }
endif      {  return MC_ENDIF; }



{idf}    {
	     yylval.vstr = strdup(yytext);
	     return T_IDF;
         }

.        { return yytext[0]; }
\n       {  }





%%

int yywrap()
{
    return 1;
}
