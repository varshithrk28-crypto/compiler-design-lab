%code requires {
    struct node;
}

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
    char *data;
    struct node *left;
    struct node *right;
};

static char *copy_text(const char *s)
{
    size_t n = strlen(s) + 1;
    char *p = (char *)malloc(n);
    if (p) memcpy(p, s, n);
    return p;
}

static struct node *create_node(const char *data,
                                struct node *left,
                                struct node *right)
{
    struct node *n = (struct node *)malloc(sizeof(struct node));
    if (!n) {
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }
    n->data = copy_text(data);
    n->left = left;
    n->right = right;
    return n;
}

static void print_tree(const struct node *root, int depth)
{
    if (!root) return;
    for (int i = 0; i < depth; i++) printf("  ");
    printf("%s\n", root->data);
    print_tree(root->left, depth + 1);
    print_tree(root->right, depth + 1);
}

static void free_tree(struct node *root)
{
    if (!root) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root->data);
    free(root);
}

int yylex(void);
void yyerror(const char *s);
struct node *root = NULL;
%}

%union {
    char *text;
    struct node *ptr;
}

%token <text> NUM ID
%type <ptr> expr
%left '+' '-'
%left '*' '/'
%right UMINUS

%%
input:
      expr { root = $1; }
    ;

expr:
      expr '+' expr       { $$ = create_node("+", $1, $3); }
    | expr '-' expr       { $$ = create_node("-", $1, $3); }
    | expr '*' expr       { $$ = create_node("*", $1, $3); }
    | expr '/' expr       { $$ = create_node("/", $1, $3); }
    | '(' expr ')'        { $$ = $2; }
    | '-' expr %prec UMINUS { $$ = create_node("unary-", NULL, $2); }
    | NUM                 { $$ = create_node($1, NULL, NULL); free($1); }
    | ID                  { $$ = create_node($1, NULL, NULL); free($1); }
    ;
%%

void yyerror(const char *s)
{
    fprintf(stderr, "Invalid expression: %s\n", s);
}

int main(void)
{
    printf("Enter an arithmetic expression (then Ctrl+Z, Enter on Windows):\n");
    if (yyparse() == 0 && root) {
        printf("Abstract Syntax Tree:\n");
        print_tree(root, 0);
    }
    free_tree(root);
    return 0;
}
