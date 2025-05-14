/* ************************************************************************** */
/*                                                                            */
/*                          Missing parts for VBC parser                      */
/*                                                                            */
/* ************************************************************************** */


// Fonction parse_factor : gère les chiffres et les parenthèses
node *parse_factor(char **s)
{
    if (isdigit(**s))
    {
        node n = { VAL, **s - '0', NULL, NULL };
        (*s)++;
        return new_node(n);
    }
    if (accept(s, '('))
    {
        node *n = parse_expr(s);
        if (!expect(s, ')'))
        {
            destroy_tree(n);
            return NULL;
        }
        return n;
    }
    unexpected(**s);
    return NULL;
}

// Fonction parse_term : gère les multiplications
node *parse_term(char **s)
{
    node *left = parse_factor(s);
    if (!left)
        return NULL;

    while (accept(s, '*'))
    {
        node *right = parse_factor(s);
        if (!right)
        {
            destroy_tree(left);
            return NULL;
        }
        node n = { MULTI, 0, left, right };
        left = new_node(n);
        if (!left)
        {
            destroy_tree(right);
            return NULL;
        }
    }
    return left;
}

// Fonction parse_expr : gère les additions
node *parse_expr(char **s)
{
    node *left = parse_term(s);
    if (!left)
        return NULL;

    while (accept(s, '+'))
    {
        node *right = parse_term(s);
        if (!right)
        {
            destroy_tree(left);
            return NULL;
        }
        node n = { ADD, 0, left, right };
        left = new_node(n);
        if (!left)
        {
            destroy_tree(right);
            return NULL;
        }
    }
    return left;
}
/*
=== ETAPE 1 ===
1.1 modifie <malloc.h> par <stdlib.h>
1.2 dans int accept , rajouter : if (**s == c)
1.3  efface complet :  node    *parse_expr(char *s)


=== ETAPE 2 ===
code ses 3 fonctions juste avant le main :
1 --> node *parse_factor(char **s)
2 --> node *parse_term(char **s)
3 --> node *parse_expr(char **s)
4 --> rajoute le prototype node *parse_expr(char **s); après la struct

=== ETAPE 3 ===
Modifie le main original:

3.1 char *s = argv[1];     			<-------- rajoute
3.2 node *tree = parse_expr(&s);	<-------- modif

3.3   rajoute cette condition après if (!tree) return (1) :

if (*s) 							<--------  Ajout du contrôle des tokens restants
{
	unexpected(*s);
	destroy_tree(tree);
	return (1);
}

**********************************************
exemple main complet :---->
int main(int argc, char **argv)
{
    if (argc != 2)
        return (1);
	char *s = argv[1];
    node *tree = parse_expr(&s);
    if (!tree)
        return (1);
	if (*s) // ← Ajout du contrôle des tokens restants
	{
		unexpected(*s);
		destroy_tree(tree);
		return (1);
	}
    printf("%d\n", eval_tree(tree));
    destroy_tree(tree);
}
***********************************************/

