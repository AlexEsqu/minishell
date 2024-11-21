/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 17:51:52 by mkling            #+#    #+#             */
/*   Updated: 2024/10/04 15:39:04 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../inc/libft.h"

size_t	countword(const char *str, char sep)
{
	size_t	i;
	size_t	num_word;

	i = 0;
	num_word = 0;
	while (str[i] != '\0')
	{
		if ((i == 0 && (str[i] != sep))
			|| (i > 0 && ((str[i - 1] == sep) && str[i] != sep)))
			num_word++;
		i++;
	}
	return (num_word);
}

static char	*fillword(const char *src, char sep, size_t *str_index)
{
	size_t	i;
	char	*word;

	i = 0;
	while (src[i] && src[i] != sep)
		i++;
	word = malloc((i + 1) * sizeof(char));
	if (!word)
		return (NULL);
	i = 0;
	while (src[i] && src[i] != sep)
	{
		word[i] = src[i];
		i++;
	}
	word[i] = '\0';
	*str_index = *str_index + i;
	return (word);
}

static void	*free_array(char **array_to_clear, size_t str_count)
{
	while (str_count > 0)
	{
		str_count--;
		free(array_to_clear[str_count]);
	}
	free(array_to_clear);
	return (NULL);
}

char	**ft_split(char const *s, char sep)
{
	size_t	i;
	size_t	current_word_count;
	char	**result;

	result = ft_calloc((countword(s, sep) + 1), sizeof(char *));
	if (!result)
		return (NULL);
	i = 0;
	current_word_count = 0;
	while (current_word_count < countword(s, sep))
	{
		if (s[i] == sep)
			i++;
		if (s[i] != sep)
		{
			result[current_word_count] = fillword(&s[i], sep, &i);
			if (!result[current_word_count])
				return (free_array(result, current_word_count));
			current_word_count++;
		}
	}
	result[countword(s, sep)] = NULL;
	return (result);
}
