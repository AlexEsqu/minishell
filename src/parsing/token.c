/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 18:04:03 by alex              #+#    #+#             */
/*   Updated: 2024/12/30 13:24:33 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	token_is(int lexem, t_list *node)
{
	if (!node || !node->content)
		return (0);
	return (((t_token *)node->content)->lexem == lexem);
}

t_token	*create_token(t_shell *shell, int lexem, char letter, char *content)
{
	t_token	*token;

	token = (t_token *)ft_calloc(sizeof(t_token), 1);
	if (!token)
		return (set_error(MALLOC_FAIL, shell, "Failed to alloc token"), NULL);
	token->letter = letter;
	token->lexem = lexem;
	if (content)
		token->content = content;
	return (token);
}

void	add_token(t_shell *shell, t_list **dest, char letter, char *content)
{
	t_token	*token;

	token = create_token(shell, letter, letter, content);
	ft_lstadd_back(dest, ft_lstnew(token));
}

void	merge_token(t_shell *shell, t_list *start)
{
	t_token	*current;
	t_token	*next;

	current = ((t_token *)start->content);
	next = ((t_token *)start->next->content);
	current->content = ft_strjoinfree(current->content, next->content);
	if (!current->content)
		return (set_error(MALLOC_FAIL, shell, "Failed to malloc token"));
	ft_lstpop(&shell->token_list, start->next, free_token);
}

void	apply_to_list(t_shell *shell, t_list *node,
			void function(t_shell *, t_list *))
{
	while (node != NULL)
	{
		function(shell, node);
		node = node->next;
	}
}
