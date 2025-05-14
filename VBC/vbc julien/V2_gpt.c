#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
    enum
	{
        ADD,
        MULTI,
        VAL
    } type;
    int     val;
    struct node   *l;
    struct node   *r;
} node;

node    *new_node(node n);
void    unexpected(char c);
int     accept(char **s, char c);
int     expect(char **s, char c);
int     eval_tree(node *tree);
node    *parse_expr(char **s);
void    destroy_tree(node* tree);

node *parse_nbr(char **s)
{
    if(!isdigit(**s))
    {
        unexpected(**s);
        return NULL;
    }
    node n = { .type = VAL, .val = **s - '0', .l = NULL, .r = NULL };
    (*s)++;
    return new_node(n);
}

node *parse_parenth(char **s)
{
    if (accept(s, '('))
    {
        node *expr = parse_expr(s);
        if(expect(s, ')'))
            return expr;
        else
        {
            destroy_tree(expr);
            return NULL;
        }
    }
    return parse_nbr(s); // If it's not a parenthesis, it's a number
}

node *parse_term(char **s)
{
    node *left = parse_parenth(s);  // Handle parentheses and numbers
    while (accept(s, '*'))
    {
        node *right = parse_parenth(s);
        if (!right)
        {
            destroy_tree(left);
            return NULL;
        }
        node n = { .type = MULTI, .l = left, .r = right };
        left = new_node(n);
    }
    return left;
}

node *parse_expr(char **s)
{
    node *left = parse_term(s);  // First parse multiplication terms
    while (accept(s, '+'))
    {
        node *right = parse_term(s);
        if(!right)
        {
            destroy_tree(left);
            return NULL;
        }
        node n = { .type = ADD, .l = left, .r = right };
        left = new_node(n);
    }
    return left;
}

node *new_node(node n)
{
    node *ret = calloc(1, sizeof(node));
    if (!ret)
        return NULL;
    *ret = n;
    return ret;
}

void unexpected(char c)
{
    if (c)
        printf("Unexpected token '%c'\n", c);
    else
        printf("Unexpected end of input\n");
}

int accept(char **s, char c)
{
    if (**s == c)
    {
        (*s)++;
        return 1;
    }
    return 0;
}

int expect(char **s, char c)
{
    if (accept(s, c))
        return 1;
    unexpected(**s);
    return 0;
}

int eval_tree(node *tree)
{
    switch (tree->type)
    {
        case ADD:
            return (eval_tree(tree->l) + eval_tree(tree->r));
        case MULTI:
            return (eval_tree(tree->l) * eval_tree(tree->r));
        case VAL:
            return tree->val;
        default:
            return 0;
    }
}

void destroy_tree(node* tree)
{
    if (tree) {
        if (tree->type != VAL) {
            destroy_tree(tree->l);
            destroy_tree(tree->r);
        }
        free(tree);
    }
}

int main(int ac, char **av)
{
    if (ac != 2)
        return 1;
    char *input = av[1];
    node *tree = parse_expr(&input);
    if (!tree)
        return 1;
    else if (*input) // Check if there's remaining input
    {
        unexpected(*input);
        destroy_tree(tree);
        return 1;
    }
    printf("%d\n", eval_tree(tree));
    destroy_tree(tree);
}
