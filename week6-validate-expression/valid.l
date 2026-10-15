%{
#include "valid.tab.h"
%}

%option noyywrap

%%
[0-9]+                  { return NUM; }
[a-zA-Z_][a-zA-Z0-9_]*  { return ID; }
[ \t\r\n]+              ;
.                       { return (unsigned char)yytext[0]; }
%%
