%{
#include <stdio.h>

int yylex(void);
void yyerror(const char *s);
%}

%define api.value.type {int}
%token NUM
%left '+' '-'
%left '*' '/'
%right UMINUS

%%
input:
      expr { printf("Result = %d\n", $1); }
    ;

expr:
      expr '+' expr          { $$ = $1 + $3; }
    | expr '-' expr          { $$ = $1 - $3; }
    | expr '*' expr          { $$ = $1 * $3; }
    | expr '/' expr          {
                                 if ($3 == 0) {
                                     yyerror("division by zero");
                                     YYABORT;
                                 }
                                 $$ = $1 / $3;
                             }
    | '(' expr ')'           { $$ = $2; }
    | '-' expr %prec UMINUS  { $$ = -$2; }
    | NUM                    { $$ = $1; }
    ;
%%

void yyerror(const char *s)
{
    fprintf(stderr, "Error: %s\n", s);
}

int main(void)
{
    printf("Enter an arithmetic expression (then Ctrl+Z, Enter on Windows):\n");
    return yyparse() == 0 ? 0 : 1;
}
