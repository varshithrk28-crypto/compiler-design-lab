%{
#include "calc.tab.h"
#include <stdlib.h>
%}

%option noyywrap

%%
[0-9]+      { yylval = atoi(yytext); return NUM; }
[ \t\r\n]+  ;
.           { return (unsigned char)yytext[0]; }
%%
