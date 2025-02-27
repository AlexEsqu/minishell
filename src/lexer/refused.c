/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   refused.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 11:54:14 by mkling            #+#    #+#             */
/*   Updated: 2025/02/27 10:36:58 by mkling           ###   ########.fr       */
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

int	input_contains_unsupported(t_shell *shell, char *input)
{
	if (is_missing_delimiter(shell, input))
		return (1);
	return (0);
}

int	redirection_after_pipe_or_and(t_list *first, t_list *second)
{
	if (((t_token *)first->content)->letter == '&'
		|| ((t_token *)first->content)->letter == '|')
	{
		if (((t_token *)second->content)->letter == '<'
			|| ((t_token *)second->content)->letter == '>')
			return (1);
	}
	return (0);
}
