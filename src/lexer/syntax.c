/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 16:42:40 by mkling            #+#    #+#             */
/*   Updated: 2025/02/20 18:54:34 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

/* Bash syntax expect word anytime after redirection to be file path */
static void	is_missing_redirection(t_shell *shell, t_list *node)
{
	t_token	*operator;
	t_list	*current;

	operator = (t_token *)node->content;
	if (shell->critical_er || !token_is(OPERATOR, node)
		|| (operator->letter != '<' && operator->letter != '>'))
		return ;
	current = node->next;
	while (!token_is(END, current))
	{
		if (token_is(WORD, current) || token_is(STRING, current))
			return ;
		current = current->next;
	}
	print_syntax_error(shell, (t_token *)current->content);
	return ;
}

static void	is_missing_cmd_before_pipe_or_amp(t_shell *shell, t_list *node)
{
	t_list	*current;

	if (shell->critical_er || !token_is(OPERATOR, node)
		|| (((t_token *)node->content)->letter != '|'
			&& ((t_token *)node->content)->letter != '&'))
		return ;
	current = node->prev;
	while (current->prev)
	{
		if (token_is(PIPE, current) || token_is(AND, current))
			return (print_syntax_error(shell, ((t_token *)node->content)));
		if (token_is(WORD, current))
			return ;
		current = current->prev;
	}
	print_syntax_error(shell, ((t_token *)node->content));
}

static void	is_missing_cmd_after_pipe_or_amp(t_shell *shell, t_list *node)
{
	t_list	*current;

	if (shell->critical_er || !token_is(OPERATOR, node)
		|| (((t_token *)node->content)->letter != '|'
			&& ((t_token *)node->content)->letter != '&'))
		return ;
	current = node->next;
	while (current->next)
	{
		if (token_is(AND, current) || token_is(PIPE, current))
			return (print_syntax_error(shell, ((t_token *)node->content)));
		if (token_is(WORD, current))
			return ;
		current = current->next;
	}
	print_syntax_error(shell, ((t_token *)node->content));
}

int	check_syntax(t_shell *shell, t_list *node)
{
	apply_to_list(shell, node, is_missing_cmd_before_pipe_or_amp);
	apply_to_list(shell, node, is_missing_cmd_after_pipe_or_amp);
	apply_to_list(shell, node, is_missing_redirection);
	return (shell->critical_er);
}
