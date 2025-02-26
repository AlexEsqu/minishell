/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delimiters.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 11:29:43 by mkling            #+#    #+#             */
/*   Updated: 2025/02/26 13:41:17 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_char_in_string(char *string, int c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (string[i])
	{
		if (string[i] == c)
			count++;
		i++;
	}
	return (count);
}

static void	remove_char_from_string(t_shell *shell, char **ptr_to_string, int c)
{
	char	*original;
	char	*result;
	size_t	char_count;
	size_t	i;
	size_t	j;

	original = *ptr_to_string;
	char_count = count_char_in_string(original, c);
	result = ft_calloc(sizeof(char), ft_strlen(original) - (char_count) + 1);
	if (!result)
		return (set_error(MALLOC_FAIL, shell));
	i = 0;
	j = 0;
	while (original[i])
	{
		if (original[i] != c)
			result[j++] = original[i];
		i++;
	}
	free(original);
	*ptr_to_string = result;
}

void	remove_quotes_from_string(t_shell *shell, char **ptr_to_string)
{
	if (shell->critical_er || !ptr_to_string || !*ptr_to_string[0])
		return ;
	if (ft_strchr(*ptr_to_string, '\'') && (*ptr_to_string)[0] == '\''
		&& (*ptr_to_string)[ft_strlen(*ptr_to_string) - 1] == '\'')
		remove_char_from_string(shell, ptr_to_string, '\'');
	else if (ft_strchr(*ptr_to_string, '\"') && (*ptr_to_string)[0] == '\"'
		&& (*ptr_to_string)[ft_strlen(*ptr_to_string) - 1] == '\"')
		remove_char_from_string(shell, ptr_to_string, '\"');
	else
	{
		remove_char_from_string(shell, ptr_to_string, '\"');
		remove_char_from_string(shell, ptr_to_string, '\'');
	}
}

void	remove_parenthesis_from_string(t_shell *shell, char **ptr_to_string)
{
	if (shell->critical_er || !ptr_to_string || !*ptr_to_string)
		return ;
	if (ft_strchr((*ptr_to_string), '('))
		remove_char_from_string(shell, ptr_to_string, '(');
	if (ft_strchr((*ptr_to_string), ')'))
		remove_char_from_string(shell, ptr_to_string, ')');
}
