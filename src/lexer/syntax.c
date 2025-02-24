/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 16:42:40 by mkling            #+#    #+#             */
/*   Updated: 2025/02/23 12:26:48 by alex             ###   ########.fr       */
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

/* Minishell's subject states open quotes are not to be implemented */
int	is_missing_delimiter(t_shell *shell, char *input)
{
	int		s_quote_count;
	int		d_quote_count;
	int		bracket_count;

	bracket_count = 0;
	s_quote_count = 0;
	d_quote_count = 0;
	while (*input)
	{
		if (*input == '(')
			bracket_count++;
		if (*input == ')')
			bracket_count--;
		if (*input == '\'')
			s_quote_count++;
		if (*input++ == '\"')
			d_quote_count++;
		if (bracket_count < 0)
			return (set_error(SYNTAX_ERROR, shell), 1);
	}
	if (d_quote_count % 2 != 0 || s_quote_count % 2 != 0 || bracket_count != 0)
		return (set_error(SYNTAX_ERROR, shell), 1);
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
