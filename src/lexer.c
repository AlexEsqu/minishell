/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 16:42:51 by mkling            #+#    #+#             */
/*   Updated: 2024/11/21 18:49:30 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	add_to_lexer(char c, int index, t_tree *previous)
{
	t_tree	*current;

	current = previous->first;
	if (previous == NULL)
		return ;
	if (previous->first == NULL)
	{
		previous->first = current;
		return ;
	}
	else
	{
		previous->right = current;
		current->left = previous;
	}
	return (current);
}

void	id_lexical(char *line, int *index, t_tree *tree)
{

}

void	id_separator(char *line, int *index, t_tree *tree)
{
	if (line[*index] != '(' || line[*index] != ')'
		|| line[*index] != '\'' || line[*index] != '"')
		return ;
	add_to_lexer();
}

void	lex(char *line, t_tree *tree)
{
	int	index;

	index = 0;
	while (line[index])
	{
		id_separator(line, &index, tree);
	}
}
