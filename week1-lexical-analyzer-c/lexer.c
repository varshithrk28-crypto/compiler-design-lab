#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define INPUT_SIZE 2000
#define TOKEN_SIZE 256

static const char *keywords[] = {
    "int", "float", "double", "char", "void", "if", "else",
    "while", "for", "return", "break", "continue"
};

static int is_keyword(const char *word)
{
    size_t count = sizeof(keywords) / sizeof(keywords[0]);
    for (size_t i = 0; i < count; i++)
        if (strcmp(word, keywords[i]) == 0)
            return 1;
    return 0;
}

int main(void)
{
    char input[INPUT_SIZE];
    size_t i = 0;

    printf("Enter one line of code:\n");
    if (!fgets(input, sizeof(input), stdin))
        return 1;

    while (input[i] != '\0')
    {
        char token[TOKEN_SIZE];
        size_t j = 0;

        if (isspace((unsigned char)input[i]))
        {
            i++;
            continue;
        }

        /* Ignore // comments */
        if (input[i] == '/' && input[i + 1] == '/')
            break;

        /* Ignore block comments */
        if (input[i] == '/' && input[i + 1] == '*')
        {
            i += 2;
            while (input[i] != '\0' &&
                   !(input[i] == '*' && input[i + 1] == '/'))
                i++;

            if (input[i] == '\0')
                printf("Warning: unterminated comment\n");
            else
                i += 2;
            continue;
        }

        /* Identifier or keyword */
        if (isalpha((unsigned char)input[i]) || input[i] == '_')
        {
            while ((isalnum((unsigned char)input[i]) || input[i] == '_') &&
                   j < TOKEN_SIZE - 1)
                token[j++] = input[i++];

            /* Skip any remaining characters in an overlong identifier */
            while (isalnum((unsigned char)input[i]) || input[i] == '_')
                i++;

            token[j] = '\0';
            printf("%s - %s\n", token,
                   is_keyword(token) ? "Keyword" : "Identifier");
            continue;
        }

        /* Integer or decimal number */
        if (isdigit((unsigned char)input[i]))
        {
            int dot_seen = 0;
            while ((isdigit((unsigned char)input[i]) ||
                    (input[i] == '.' && !dot_seen)) &&
                   j < TOKEN_SIZE - 1)
            {
                if (input[i] == '.')
                    dot_seen = 1;
                token[j++] = input[i++];
            }
            token[j] = '\0';
            printf("%s - Number\n", token);
            continue;
        }

        /* Recognize common two-character operators */
        if ((input[i] == '=' && input[i + 1] == '=') ||
            (input[i] == '!' && input[i + 1] == '=') ||
            (input[i] == '<' && input[i + 1] == '=') ||
            (input[i] == '>' && input[i + 1] == '=') ||
            (input[i] == '+' && input[i + 1] == '+') ||
            (input[i] == '-' && input[i + 1] == '-') ||
            (input[i] == '&' && input[i + 1] == '&') ||
            (input[i] == '|' && input[i + 1] == '|'))
        {
            printf("%c%c - Operator\n", input[i], input[i + 1]);
            i += 2;
            continue;
        }

        if (strchr("+-*/%=<>!&|", input[i]))
            printf("%c - Operator\n", input[i]);
        else if (strchr(";,(){}[]", input[i]))
            printf("%c - Special Symbol\n", input[i]);
        else
            printf("%c - Unknown\n", input[i]);

        i++;
    }

    return 0;
}
