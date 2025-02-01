/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 16:26:26 by mkling            #+#    #+#             */
/*   Updated: 2025/01/30 16:28:05 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	echo(char **argv)
{
	int		i;
	bool	need_newline;

	need_newline = true;
	i = 1;
	if (argv[i] && ft_strcmp(argv[i], "-n") == 0)
	{
		i++;
		need_newline = false;
	}
	while (argv[i] != NULL)
	{
		if ((need_newline && i > 1) || (!need_newline && i > 2))
			ft_putstr_fd(" ", STDOUT_FILENO);
		ft_putstr_fd(argv[i], STDOUT_FILENO);
		i++;
	}
	if (need_newline)
		write(1, "\n", STDOUT_FILENO);
	// ft_free_tab(argv);
	return (0);
}
