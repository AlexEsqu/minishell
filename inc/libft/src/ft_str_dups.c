/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 18:03:46 by mkling            #+#    #+#             */
/*   Updated: 2024/10/04 15:54:11 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../inc/libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*result;
	size_t	len;

	len = ft_strlen(s1) + ft_strlen(s2) + 1;
	result = ft_calloc(len, sizeof(char));
	if (!result)
		return (NULL);
	ft_memcpy(result, s1, ft_strlen(s1));
	ft_strlcat(result, s2, len);
	return (result);
}

char	*ft_strdup(const char *src)
{
	int		i;
	char	*new;

	i = 0;
	while (src[i])
		i++;
	new = malloc((i + 1) * sizeof(char));
	if (new == NULL)
		return (NULL);
	i = 0;
	while (src[i])
	{
		new[i] = src[i];
		i++;
	}
	new[i] = '\0';
	return (new);
}

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	src_len;
	size_t	dest_len;
	size_t	i;

	i = 0;
	src_len = ft_strlen(src);
	dest_len = ft_strlen(dest);
	if (dest_len > size)
		dest_len = size;
	if (dest_len == size)
		return (size + src_len);
	while ((dest_len + i) < (size - 1) && src[i])
	{
		dest[dest_len + i] = src[i];
		i++;
	}
	dest[dest_len + i] = '\0';
	return (dest_len + src_len);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char			*result;
	unsigned int	i;

	if (!s)
		return (0);
	if (start > ft_strlen(s))
		return (ft_strdup(""));
	if (len > ft_strlen(s + start))
		len = ft_strlen(s + start);
	result = malloc((len + 1) * sizeof(char));
	if (!result)
		return (NULL);
	i = 0;
	while (i < len)
	{
		result[i] = s[start + i];
		i++;
	}
	result[i] = '\0';
	return (result);
}

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	if (size != 0)
	{
		while (i < size - 1 && src[i] != '\0')
		{
			dest[i] = src[i];
			i++;
		}
		dest[i] = '\0';
	}
	while (src[i])
		i++;
	return (i);
}
