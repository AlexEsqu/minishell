/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgodoy <vgodoy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 18:09:56 by vgodoy            #+#    #+#             */
/*   Updated: 2025/02/19 18:09:58 by vgodoy           ###   ########.fr       */
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
		ft_putstr_fd(cmd->argv[i], STDOUT_FILENO);
		if (i + 1 < cmd->argc && cmd->argv[i + 1][0])
			ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}
	if (need_newline)
		write(1, "\n", STDOUT_FILENO);
	return (0);
}
