/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 16:42:40 by mkling            #+#    #+#             */
/*   Updated: 2025/02/24 11:53:52 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_operator_requiring_word_before(t_list *node)
{
	if (((t_token *)node->content)->letter == '&')
		return (1);
	if (((t_token *)node->content)->letter == '|')
		return (1);
	return (0);
}

int	is_operator_requiring_word_after(t_list *node)
{
	if (is_operator_requiring_word_before(node))
		return (1);
	if (((t_token *)node->content)->letter == '<')
		return (1);
	if (((t_token *)node->content)->letter == '>')
		return (1);
	return (0);
}

static void	is_missing_word_before_operator(t_shell *shell, t_list *node)
{
	t_list	*current;

	if (shell->critical_er || !is_operator_requiring_word_before(node))
		return ;
	current = node->prev;
	while (current->prev)
	{
		if (is_operator_requiring_word_before(current))
			return (print_syntax_error(shell, ((t_token *)node->content)));
		if (token_is(WORD, current))
			return ;
		current = current->prev;
	}
	print_syntax_error(shell, ((t_token *)node->content));
}

static void	is_missing_word_after_operator(t_shell *shell, t_list *node)
{
	t_list	*current;

	if (shell->critical_er || !is_operator_requiring_word_after(node))
		return ;
	current = node->next;
	while (current->next)
	{
		if (token_is(OPERATOR, current))
			return (print_syntax_error(shell, ((t_token *)node->content)));
		if (token_is(WORD, current))
			return ;
		current = current->next;
	}
	print_syntax_error(shell, ((t_token *)node->content));
}

int	check_syntax(t_shell *shell, t_list *node)
{
	apply_to_list(shell, node, is_missing_word_before_operator);
	apply_to_list(shell, node, is_missing_word_after_operator);
	return (shell->critical_er);
}
