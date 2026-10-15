%{
#include <stdio.h>
%}

%option noyywrap

%%
"//".*                                      { /* Ignore single-line comments */ }
"/*"([^*]|\*+[^*/])*\*+"/"                  { /* Ignore multi-line comments */ }

"int"|"float"|"double"|"char"|"void"|"if"|"else"|"while"|"for"|"return"|"break"|"continue" { printf("%s - Keyword\n", yytext); }

[a-zA-Z_][a-zA-Z0-9_]*                      { printf("%s - Identifier\n", yytext); }
[0-9]+(\.[0-9]+)?                           { printf("%s - Number\n", yytext); }

"=="|"!="|"<="|">="|"++"|"--"|"&&"|"||"    { printf("%s - Operator\n", yytext); }
[+\-*/%=<>!&|]                              { printf("%s - Operator\n", yytext); }
[;,(){}\[\]]                                { printf("%s - Special Symbol\n", yytext); }

[ \t\r\n]+                                  { /* Ignore whitespace */ }
.                                           { printf("%s - Unknown\n", yytext); }
%%

int main(void)
{
    printf("Enter code (Ctrl+Z then Enter on Windows, Ctrl+D on Linux):\n");
    yylex();
    return 0;
}
