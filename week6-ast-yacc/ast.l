%{
#include "ast.tab.h"
#include <stdlib.h>
#include <string.h>

static char *copy_text(const char *s)
{
    size_t n = strlen(s) + 1;
    char *p = (char *)malloc(n);
    if (p) memcpy(p, s, n);
    return p;
}
%}

%option noyywrap

%%
[0-9]+                  { yylval.text = copy_text(yytext); return NUM; }
[a-zA-Z_][a-zA-Z0-9_]*  { yylval.text = copy_text(yytext); return ID; }
[ \t\r\n]+              ;
.                       { return (unsigned char)yytext[0]; }
%%
