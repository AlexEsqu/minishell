/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   refused.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 11:54:14 by mkling            #+#    #+#             */
/*   Updated: 2025/02/25 21:20:22 by mkling           ###   ########.fr       */
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

int	contains_non_supported_char(t_shell *shell, char *input)
{
	while (*input)
	{
		if (*input == ';')
			return (set_error(SYNTAX_ERROR, shell), 1);
		// if (*input++ == '\\')
		// 	return (set_error(SYNTAX_ERROR, shell), 1);
		input++;
	}
	return (0);
}

int	input_contains_unsupported(t_shell *shell, char *input)
{
	if (contains_non_supported_char(shell, input))
		return (1);
	if (is_missing_delimiter(shell, input))
		return (1);
	return (0);
}
