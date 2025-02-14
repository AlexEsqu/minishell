/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 16:26:26 by mkling            #+#    #+#             */
/*   Updated: 2025/02/14 00:55:50 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	echo(t_cmd *cmd)
{
	bool	need_newline;
	int		i;

	need_newline = true;
	i = 1;
	if (cmd->argv[1] && ft_strcmp(cmd->argv[1], "-n") == 0)
	{
		need_newline = false;
		i++;
	}
	while (i < cmd->argc)
	{
		if (cmd->argv[i] && cmd->argv[i][0])
		{
			ft_putstr_fd(cmd->argv[i], STDOUT_FILENO);
			if (i + 1 < cmd->argc && cmd->argv[i + 1][0])
				ft_putstr_fd(" ", STDOUT_FILENO);
		}
		i++;
	}
	if (need_newline)
		write(1, "\n", STDOUT_FILENO);
	return (0);
}
