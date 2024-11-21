/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 11:03:13 by mkling            #+#    #+#             */
/*   Updated: 2024/11/21 11:19:04 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	i = 0;
	while (s[i])
	{
		write(fd, s[i], 1);
		i++;
	}
}

void	exit_if(bool condition, char *err_message, int err_code, void *data)
{
	if (condition == true)
	{
		free(data);
		ft_putstr_fd(2, err_message);
		exit(err_code);
	}
}
