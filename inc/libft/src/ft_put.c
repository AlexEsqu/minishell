/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_put.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 18:39:39 by mkling            #+#    #+#             */
/*   Updated: 2024/12/16 17:17:38 by mkling           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <unistd.h>
#include "../inc/libft.h"

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

void	ft_putendl_fd(char *s, int fd)
{
	ft_putstr_fd(s, fd);
	ft_putchar_fd('\n', fd);
}

void	ft_putnbr_fd(int n, int fd)
{
	long	num;

	num = n;
	if (num < 0)
	{
		ft_putchar_fd('-', fd);
		num = num * -1;
	}
	if (num <= 9)
		ft_putchar_fd(('0' + num), fd);
	if (num > 9)
	{
		ft_putnbr_fd((num / 10), fd);
		ft_putchar_fd('0' + (num % 10), fd);
	}
}

void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	i = 0;
	if (s == NULL)
		return ;
	while (s[i])
	{
		ft_putchar_fd(s[i], fd);
		i++;
	}
}
