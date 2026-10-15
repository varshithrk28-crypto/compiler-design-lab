%{
#include <stdio.h>

int yylex(void);
void yyerror(const char *s);
%}

%token NUM ID
%left '+' '-'
%left '*' '/'
%right UMINUS

%%
input:
      expr { puts("Valid arithmetic expression"); }
    ;

expr:
      expr '+' expr
    | expr '-' expr
    | expr '*' expr
    | expr '/' expr
    | '(' expr ')'
    | '-' expr %prec UMINUS
    | NUM
    | ID
    ;
%%

void yyerror(const char *s)
{
    puts("Invalid arithmetic expression");
}

int main(void)
{
    printf("Enter an arithmetic expression (then Ctrl+Z, Enter on Windows):\n");
    return yyparse() == 0 ? 0 : 1;
}
