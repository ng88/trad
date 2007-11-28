
%{

#include <stdio.h>
#include <stdlib.h>
#include  <string.h>

#include "anasyn.tab.h"
#include "anasyn.h"

char * process_backslashes(char * str);

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

                                            yylval.vstr = process_backslashes(yytext + 1);

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

=          {  return OP_EQ; }
!=         {  return OP_NE; }
\<=        {  return OP_LE; }
>=         {  return OP_GE; }
\<         {  return OP_LT; }
>          {  return OP_GT; }
&          {  return OP_AND; }
\|\|       {  return OP_OR; }
\+         {  return OP_PLUS; }
-          {  return OP_MINUS; }
\/         {  return OP_DIV; }
\*         {  return OP_MUL; }
:=         {  return OP_AFFECT; }
\(         {  return OP_BRACKET_O; }
\)         {  return OP_BRACKET_C; }
\{         {  return OP_BRACE_O; }
\}         {  return OP_BRACE_C; }
\;         {  return OP_SEMICOLON; }
,          {  return OP_COMMA; }
\.         {  return OP_MEMBER; }

class      {  return MC_CLASS; }
end        {  return MC_END; }
inherit    {  return MC_INHERIT; }
private    {  return MC_PRIVATE; }
public     {  return MC_PUBLIC; }
integer    {  return MC_INTEGER; }
string     {  return MC_STRING; }
void       {  return MC_VOID; }
super      {  return MC_SUPER; }
return     {  return MC_RETURN; }
new        {  return MC_NEW; }
while      {  return MC_WHILE; }
do         {  return MC_DO; }
if         {  return MC_IF; }
then       {  return MC_THEN; }
else       {  return MC_ELSE; }
endif      {  return MC_ENDIF; }
VAR        {  return MC_VAR; }



{idf}    {
             if(strlen(yytext) > 20)
	     {
		 yyerror("identificateur trop long");
	     }
	     yylval.vstr = strdup(yytext);
	     return T_IDF;
         }


[\t\n ]      {  }
.        { return yytext[0]; }





%%

int yywrap()
{
    return 1;
}

char * process_backslashes(char * str)
{
    int n = strlen(str);
    char * dest = (char*)malloc(n + 1);

    int i;
    int j = 0;
		
    for(i = 0; i < n; ++i)
    {
	if(str[i] == '\\')
	{
	    /* on est sur que i+1 existe */
	    switch(str[i + 1])
	    {
	    case 'n':
		dest[j++] = '\n';
		i++;
		break;
	    case 't':
		dest[j++] = '\t';
		i++;
		break;
	    case '\\':
		dest[j++] = '\\';
		i++;
		break;
	    case '"':
		dest[j++] = '\"';
		i++;
		break;
	    default:
		//free(dest);
		printf("attention: \\%c n'est pas échappement valide\n", 
		       str[i + 1]);
	    }
	}
	else
	    dest[j++] = str[i];
    }

    dest[n] = '\0';

    return dest;
}
