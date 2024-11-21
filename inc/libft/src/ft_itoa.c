/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:25:11 by mkling            #+#    #+#             */
/*   Updated: 2024/10/04 15:37:09 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../inc/libft.h"

static int	nbrlen(long n)
{
	int	len;

	len = 0;
	if (n < 0)
	{
		len++;
		n *= -1;
	}
	while (n >= 10)
	{
		n = n / 10;
		len++;
	}
	if (n <= 9)
		len++;
	return (len);
}

static char	*catchar(char *dest, char c)
{
	int	i;

	i = 0;
	while (dest[i] != '\0')
		i++;
	dest[i] = c;
	dest[i + 1] = '\0';
	return (dest);
}

static void	catnbr(long num, char *result)
{
	if (num < 0)
	{
		catchar(result, '-');
		num = num * -1;
	}
	if (num <= 9)
		catchar(result, ('0' + num));
	if (num > 9)
	{
		catnbr((num / 10), result);
		catchar(result, '0' + (num % 10));
	}
}

char	*ft_itoa(int n)
{
	char	*result;

	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	result = malloc((nbrlen(n) + 1) * sizeof(char));
	if (!result)
		return (NULL);
	result[0] = '\0';
	catnbr(n, result);
	return (result);
}
