#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_PROD 100
#define MAX_RHS  100
#define ASCII    128

typedef struct {
    char lhs;
    char rhs[MAX_RHS];
} Production;

Production p[MAX_PROD];
int production_count;
int is_nt[ASCII];
int first_set[ASCII][ASCII];
int follow_set[ASCII][ASCII];
char nonterminals[ASCII];
int nt_count;

static void add_to_set(int set[ASCII][ASCII], char symbol, char value)
{
    unsigned char s = (unsigned char)symbol;
    unsigned char v = (unsigned char)value;
    if (s < ASCII && v < ASCII)
        set[s][v] = 1;
}

static int has_epsilon_first(char nt)
{
    return first_set[(unsigned char)nt]['#'];
}

static void add_nonterminal(char c)
{
    unsigned char u = (unsigned char)c;
    if (u < ASCII && !is_nt[u]) {
        is_nt[u] = 1;
        nonterminals[nt_count++] = c;
    }
}

static int is_nonterminal(char c)
{
    unsigned char u = (unsigned char)c;
    return u < ASCII && is_nt[u];
}

/* FIRST of a sequence, adding terminals to result.
   Returns 1 if the entire sequence can derive epsilon. */
static int first_of_sequence(const char *seq, int result[ASCII])
{
    int all_nullable = 1;

    if (seq[0] == '\0' || (seq[0] == '#' && seq[1] == '\0')) {
        result['#'] = 1;
        return 1;
    }

    for (size_t i = 0; seq[i] != '\0'; i++) {
        char symbol = seq[i];

        if (symbol == '#') {
            result['#'] = 1;
            return 1;
        }

        if (!is_nonterminal(symbol)) {
            result[(unsigned char)symbol] = 1;
            return 0;
        }

        for (int c = 0; c < ASCII; c++)
            if (c != '#' && first_set[(unsigned char)symbol][c])
                result[c] = 1;

        if (!has_epsilon_first(symbol)) {
            all_nullable = 0;
            break;
        }
    }

    if (all_nullable)
        result['#'] = 1;
    return all_nullable;
}

static void compute_first(void)
{
    int changed;
    do {
        changed = 0;

        for (int i = 0; i < production_count; i++) {
            char lhs = p[i].lhs;
            int before[ASCII] = {0};
            for (int c = 0; c < ASCII; c++)
                before[c] = first_set[(unsigned char)lhs][c];

            int result[ASCII] = {0};
            first_of_sequence(p[i].rhs, result);

            for (int c = 0; c < ASCII; c++)
                if (result[c])
                    first_set[(unsigned char)lhs][c] = 1;

            for (int c = 0; c < ASCII; c++)
                if (before[c] != first_set[(unsigned char)lhs][c]) {
                    changed = 1;
                    break;
                }
        }
    } while (changed);
}

static void compute_follow(void)
{
    /* The first production's LHS is the start symbol. */
    add_to_set(follow_set, p[0].lhs, '$');

    int changed;
    do {
        changed = 0;

        for (int i = 0; i < production_count; i++) {
            char lhs = p[i].lhs;
            const char *rhs = p[i].rhs;

            for (size_t j = 0; rhs[j] != '\0'; j++) {
                char symbol = rhs[j];
                if (!is_nonterminal(symbol))
                    continue;

                int before[ASCII] = {0};
                for (int c = 0; c < ASCII; c++)
                    before[c] = follow_set[(unsigned char)symbol][c];

                int suffix_first[ASCII] = {0};
                first_of_sequence(rhs + j + 1, suffix_first);

                for (int c = 0; c < ASCII; c++)
                    if (c != '#' && suffix_first[c])
                        follow_set[(unsigned char)symbol][c] = 1;

                if (suffix_first['#']) {
                    for (int c = 0; c < ASCII; c++)
                        if (follow_set[(unsigned char)lhs][c])
                            follow_set[(unsigned char)symbol][c] = 1;
                }

                for (int c = 0; c < ASCII; c++)
                    if (before[c] != follow_set[(unsigned char)symbol][c]) {
                        changed = 1;
                        break;
                    }
            }
        }
    } while (changed);
}

static void print_set(const char *label, char nt, int set[ASCII][ASCII])
{
    printf("%s(%c) = { ", label, nt);
    int first = 1;
    for (int c = 0; c < ASCII; c++) {
        if (set[(unsigned char)nt][c]) {
            if (!first)
                printf(", ");
            if (c == '#')
                printf("#");
            else
                printf("%c", c);
            first = 0;
        }
    }
    printf(" }\n");
}

int main(void)
{
    char line[256];

    printf("Enter number of productions: ");
    if (scanf("%d", &production_count) != 1 ||
        production_count < 1 || production_count > MAX_PROD) {
        printf("Invalid number of productions.\n");
        return 1;
    }

    printf("Enter productions one per line (example E=TR or E->TR; use # for epsilon):\n");
    for (int i = 0; i < production_count; i++) {
        if (scanf("%255s", line) != 1) {
            printf("Could not read production.\n");
            return 1;
        }

        char *arrow = strstr(line, "->");
        char *equals = strchr(line, '=');
        char *sep = arrow ? arrow : equals;

        if (!sep || !isupper((unsigned char)line[0])) {
            printf("Invalid production: %s\n", line);
            return 1;
        }

        p[i].lhs = line[0];
        const char *rhs_start = sep + (arrow ? 2 : 1);
        if (strlen(rhs_start) >= MAX_RHS) {
            printf("Right-hand side too long.\n");
            return 1;
        }
        strcpy(p[i].rhs, rhs_start);
        if (p[i].rhs[0] == '\0')
            strcpy(p[i].rhs, "#");

        add_nonterminal(p[i].lhs);
    }

    compute_first();
    compute_follow();

    printf("\nFIRST sets:\n");
    for (int i = 0; i < nt_count; i++)
        print_set("FIRST", nonterminals[i], first_set);

    printf("\nFOLLOW sets:\n");
    for (int i = 0; i < nt_count; i++)
        print_set("FOLLOW", nonterminals[i], follow_set);

    return 0;
}
