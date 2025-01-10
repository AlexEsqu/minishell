/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 14:47:23 by mkling            #+#    #+#             */
/*   Updated: 2024/11/30 00:02:50 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/libft.h"

int	ft_islower(int c)
{
	if (c >= 'a' && c <= 'z')
		return (1024);
	return (0);
}

int	ft_isupper(int c)
{
	if (c >= 'A' && c <= 'Z')
		return (1024);
	return (0);
}

int	ft_isalpha(int c)
{
	if (ft_islower(c) || ft_isupper(c))
		return (1024);
	return (0);
}

int	ft_isalnum(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
		|| (c >= '0' && c <= '9'))
		return (8);
	return (0);
}

int	ft_ishexa(int c)
{
	if (((c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
		|| (c >= '0' && c <= '9'))
		return (1);
	return (0);
}
