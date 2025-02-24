/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delimiters.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 11:29:43 by mkling            #+#    #+#             */
/*   Updated: 2025/02/24 11:27:22 by mkling           ###   ########.fr       */
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
	char	*original_string;
	char	*result_string;
	size_t	char_count;
	size_t	i;
	size_t	j;

	original_string = *ptr_to_string;
	char_count = count_char_in_string(original_string, c);
	result_string = ft_calloc(sizeof(char),
			ft_strlen(original_string) - (char_count - 1));
	if (!result_string)
		return (set_error(MALLOC_FAIL, shell));
	i = 0;
	j = 0;
	while (original_string[i])
	{
		if (original_string[i] != c)
			result_string[j++] = original_string[i];
		i++;
	}
	free(original_string);
	*ptr_to_string = result_string;
}

void	remove_quotes_from_string(t_shell *shell, char **ptr_to_string)
{
	if (shell->critical_er || !ptr_to_string || !*ptr_to_string[0])
		return ;
	if (ft_strchr((*ptr_to_string), '\'') && (*ptr_to_string)[0] == '\'')
		remove_char_from_string(shell, ptr_to_string, '\'');
	if (ft_strchr((*ptr_to_string), '\"') && (*ptr_to_string)[0] == '\"')
		remove_char_from_string(shell, ptr_to_string, '\"');
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
