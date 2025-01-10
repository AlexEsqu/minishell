/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 17:12:03 by mkling            #+#    #+#             */
/*   Updated: 2024/10/23 12:02:51 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../inc/libft.h"

static int	isinset(char const c, char const *set)
{
	while (*set)
	{
		if (*set == c)
			return (1);
		set++;
	}
	return (0);
}

static size_t	indexstart(char const *str, char const *set)
{
	unsigned int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (!isinset(str[i], set))
			return (i);
		else
			i++;
	}
	return (i);
}

static size_t	indexend(char const *str, char const *set)
{
	long	i;

	i = ft_strlen(str) - 1;
	while (i >= 0)
	{
		if (isinset(str[i], set))
			i--;
		else
			return (i);
	}
	return ((size_t)i);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*result;
	size_t	start;
	size_t	len;

	if (!set)
		return (ft_strdup(s1));
	start = indexstart(s1, set);
	len = (indexend(s1, set) - start) + 1;
	if (len == 0)
		len = 2;
	if (!s1 || start >= ft_strlen(s1))
		return (ft_strdup(""));
	result = ft_calloc(len + 1, sizeof(char));
	if (!result)
		return (NULL);
	ft_memcpy(result, &s1[start], len);
	return (result);
}
