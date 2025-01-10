/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 09:19:50 by skassimi          #+#    #+#             */
/*   Updated: 2024/12/29 10:39:21 by mkling           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

void	print_env_as_export(t_shell *shell, int fdout)
{
	t_list	*current_env;

	current_env = shell->env_list;
	while (current_env != NULL)
	{
		ft_putstr_fd("export ", fdout);
		ft_putstr_fd((char *)current_env->content, fdout);
		write(fdout, "\n", 1);
		current_env = current_env->next;
	}
}

int	export(t_shell *shell, char **argv, int fdout)
{
	int		i;
	t_list	*current_env;

	i = 1;
	if (fdout < 0)
		return (-1);
	if (argv[i] == NULL )
		return (print_env_as_export(shell, fdout), 0);
	while (argv[i])
	{
		unset(shell, &argv[i]);
		current_env = ft_lstnew(ft_strdup(argv[i]));
		if (!current_env || !current_env->content)
			return (-1);
		ft_lstadd_back(&shell->env_list, current_env);
		i++;
	}
	return (0);
}
