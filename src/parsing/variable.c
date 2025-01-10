/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrapper.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 13:50:23 by alex              #+#    #+#             */
/*   Updated: 2024/12/30 19:17:29 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	letter_is(int lexem, char *string)
{
	int	token_type;

	if (ft_strchr(DELIMITERS, *string) != NULL)
		token_type = DELIMITER;
	else if (ft_strchr(OPERATORS, *string))
	{
		if (*string == '$' && !is_valid_variable(string))
			token_type = WORD;
		else
			token_type = OPERATOR;
	}
	else if (ft_strchr(BLANKS, *string))
		token_type = BLANK;
	else
		token_type = WORD;
	return (token_type == lexem);
}

int	is_valid_variable(char *input)
{
	if (input[0] != '$' || !letter_is(WORD, &input[1]))
		return (0);
	return (1);
}

int	has_valid_var(char *string)
{
	if (!string || !string[0] || !ft_strchr(string, '$'))
		return (0);
	while (string)
	{
		if (string == '$' && is_valid_variable(&string))
			return (1);
		else
			*string++;
	}
	return (0);
}

int	can_expand(t_list *node)
{
	char	*string;

	if (!node || !node->content)
		return (0);
	string = (char *)node->content;
	if (string[0] == '\'')
		return (0);
	return (1);
}
