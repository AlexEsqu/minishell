/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 17:27:11 by mkling            #+#    #+#             */
/*   Updated: 2025/02/25 18:40:30 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/libft.h"
#include <stdlib.h>

int	ft_atoi(const char *str)
{
	int	i;
	int	minus;
	int	number;

	i = 0;
	minus = -1;
	number = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			minus = minus * (-1);
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		number = number * 10 - (str[i] - '0');
		i++;
	}
	return (number * minus);
}

long long	ft_atol(const char *str)
{
	long long	result;
	int			sign;
	int			i;

	result = 0;
	sign = -1;
	i = 0;
	while (str[i] == ' ')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = 1;
		i++;
	}
	while (ft_isdigit(str[i]))
	{
		result = result * 10 - (str[i] - '0');
		i++;
	}
	result = result * sign;
	return (result);
}

int	is_too_long_for_long_long(char *str)
{
	long long	result;
	int			sign;
	int			i;

	result = 0;
	sign = 1;
	i = 0;
	while (str[i] == ' ')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (ft_isdigit(str[i]))
	{
		if (((i == 17 && (str[i] - '0') > 8 && sign == -1)
				|| (result >= 922337203685477580 && (str[i] - '0') > 7))
			|| (i == 18 && str[i + 1]))
			return (1);
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (0);
}

int	is_too_long_for_int(char *str)
{
	int	digit_count;

	digit_count = 0;
	while (*str == ' ' || *str == '\t' || *str == '\n'
		|| *str == '\r' || *str == '\f' || *str == '\v')
		str++;
	if (*str == '-' || *str == '+')
		str++;
	while ((*str) == '0')
		str++;
	while (ft_isdigit(*str))
	{
		digit_count++;
		str++;
	}
	return (digit_count > 10);
}

int	ft_abs(int num)
{
	if (num >= 0)
		return (num);
	else
		return (-num);
}
