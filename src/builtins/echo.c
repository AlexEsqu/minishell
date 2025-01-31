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
			ft_putstr_fd(" ", 1);
		ft_putstr_fd(argv[i], 1);
		i++;
	}
	if (need_newline)
		write(1, "\n", 1);
	// ft_free_tab(argv);
	return (0);
}
