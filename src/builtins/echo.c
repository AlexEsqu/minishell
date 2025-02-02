/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 16:26:26 by mkling            #+#    #+#             */
/*   Updated: 2025/02/02 13:56:20 by mkling           ###   ########.fr       */
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
	while (cmd->argv[i] != NULL)
	{
		if ((need_newline && i > 1) || (!need_newline && i > 2))
			ft_putstr_fd(" ", STDOUT_FILENO);
		ft_putstr_fd(cmd->argv[i], STDOUT_FILENO);
		i++;
	}
	if (need_newline)
		write(1, "\n", STDOUT_FILENO);
	// ft_free_tab(argv);
	return (0);
}
