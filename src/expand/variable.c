/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 13:50:23 by alex              #+#    #+#             */
/*   Updated: 2025/02/19 15:44:43 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	letter_is(int lexem, char *string)
{
	int	token_type;

	if (ft_strchr(DELIMITERS, *string) != NULL)
		token_type = DELIMITER;
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
	int	i;

	if (!string || !string[0] || !ft_strchr(string, '$'))
		return (0);
	i = 0;
	while (string[i])
	{
		if (string[i] == '$' && is_valid_variable(&string[i]))
			return (1);
		else
			i++;
	}
	return (0);
}

void	id_variables(t_shell *shell, t_list *current)
{
	t_token	*token;
	t_token	*next_token;
	char	*tmp;

	token = (t_token *)current->content;
	if (token->lexem == DOLLAR)
	{
		next_token = (t_token *)current->next->content;
		next_token->lexem = VARIABLE;
		tmp = ft_strjoin("$", next_token->content);
		free(next_token->content);
		next_token->content = tmp;
	}
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
