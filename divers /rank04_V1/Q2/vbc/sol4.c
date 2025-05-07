/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sol4.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adiler <adiler@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 13:29:49 by cadenegr          #+#    #+#             */
/*   Updated: 2025/04/07 18:29:20 by adiler           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

typedef struct node
{
	enum{ADD, MULTI, VAL}	type;
	int						value;
	struct node				*l;
	struct node				*r;
}	node;

node	*parse_expr(char **s);

void	unexpected(char **s)
{
	if (**s == 0)
		printf("Wrong end of input\n");
	else
		printf("Wrong token %c\n", **s);
	exit(1);
}

node	*new_node(node n)
{
	node	*ret;
	ret = calloc(1, sizeof(node));
	if (!ret)
		return NULL;
	*ret = n;
	return ret;
}

int	accept(char **s, char c)
{
	if (**s == c)
	{
		(*s)++;
		return 1;
	}
	return 0;
}

int	expect(char **s, char c)
{
	if (accept(s, c))
		return 1;
	unexpected(s);
	return 0;
}

node	*parse_factor(char **s)
{
	node	n;
	if (accept(s, '('))
	{
		node	*expr = parse_expr(s);
		expect(s, ')');
		return (expr);
	}
	if (isdigit(**s))
	{
		n.type = VAL;
		n.value = **s - '0';
		(*s)++;
		return (new_node(n));
	}
	unexpected(s);
	return NULL;
}

node	*parse_term(char **s)
{
	node	*left = parse_factor(s);
	if (accept(s, '*'))
	{
		node	n;
		n.type = MULTI;
		n.l = left;
		n.r = parse_term(s);
		left = new_node(n);
	}
	return left;
}

node	*parse_expr(char **s)
{
	node	*ret = parse_term(s);
	if (accept(s, '+'))
	{
		node	n;
		n.type = ADD;
		n.l = ret;
		n.r = parse_expr(s);
		ret = new_node(n);
	}
	return ret;
}

void    destroy_tree(node *n)
{
    if (!n)
        return ;
    if (n->type != VAL)
    {
        destroy_tree(n->l);
        destroy_tree(n->r);
    }
    free(n);
}

int	eval_tree(node *tree)
{
	switch (tree->type)
	{
		case ADD:
			return (eval_tree(tree->l) + eval_tree(tree->r));
		case MULTI:
			return (eval_tree(tree->l) * eval_tree(tree->r));
		case VAL:
			return (tree->value);
	}
	return 0;
}

int	main(int ac, char **av)
{
	if (ac != 2)
		return 1;
	char *arg = av[1];
	node	*tree;
	tree = parse_expr(&arg);
	if (*arg)
	{
		destroy_tree(tree);
		unexpected(&arg);
		return 1;
	}
	if (!tree)
		return 1;
	printf("%d\n", eval_tree(tree));
	destroy_tree(tree);
	return 0;
}
