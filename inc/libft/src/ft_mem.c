/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mem.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 14:59:01 by mkling            #+#    #+#             */
/*   Updated: 2024/10/06 17:55:23 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*ptr_s;
	size_t			i;

	i = 0;
	ptr_s = (unsigned char *)s;
	while (i < n)
	{
		if (ptr_s[i] == (unsigned char)c)
			return (&ptr_s[i]);
		i++;
	}
	return (NULL);
}

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*p1;
	unsigned char	*p2;

	p1 = (unsigned char *)s1;
	p2 = (unsigned char *)s2;
	while (n > 0)
	{
		if (*p1 != *p2)
			return (*p1 - *p2);
		n--;
		p1++;
		p2++;
	}
	return (0);
}

void	*ft_memcpy(void *dest, const void *src, size_t len)
{
	unsigned long	i;
	unsigned char	*strsrc;
	unsigned char	*strdest;

	i = 0;
	strsrc = (unsigned char *)src;
	strdest = (unsigned char *)dest;
	if (src == dest)
		return (dest);
	while (len > i)
	{
		strdest[i] = strsrc[i];
		i++;
	}
	return ((void *)strdest);
}

void	*ft_memmove(void *dest, const void *src, size_t len)
{
	size_t			i;
	unsigned char	*strsrc;
	unsigned char	*strdest;

	i = 0;
	strsrc = (unsigned char *)src;
	strdest = (unsigned char *)dest;
	if (dest > src)
	{
		while (len > 0)
		{
			len--;
			strdest[len] = strsrc[len];
		}
	}
	else if (dest < src)
	{
		while (i < len)
		{
			strdest[i] = strsrc[i];
			i++;
		}
	}
	return (dest);
}

void	*ft_memset(void *ptr, int value, unsigned long len)
{
	unsigned char	*str;

	str = ptr;
	while (len--)
		*str++ = (unsigned char)value;
	return (ptr);
}
