/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 18:04:03 by alex              #+#    #+#             */
/*   Updated: 2025/02/10 16:28:05 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	token_is(int lexem, t_list *node)
{
	if (!node || !node->content)
		return (0);
	return (((t_token *)node->content)->lexem == lexem);
}

int	token_is_redirection(t_list *token_node)
{
	return (token_is(OUTFILE, token_node) || token_is(APPEND, token_node)
		|| token_is(INFILE, token_node) || token_is(HEREDOC, token_node));
}

int	token_is_operator(t_list *token_node)
{
	return (token_is(PIPE, token_node) || token_is(END, token_node)
		|| token_is(OR, token_node) || token_is(AND, token_node));
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
		return (set_error(MALLOC_FAIL, shell));
	ft_lstpop(&shell->token_list, start->next, free_token);
}


