/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 09:56:54 by mkling            #+#    #+#             */
/*   Updated: 2025/01/30 10:36:57 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_env_as_export(t_shell *shell)
{
	t_list	*current_env;

	current_env = shell->env_list;
	while (current_env != NULL)
	{
		ft_putstr_fd("export ", 1);
		ft_putstr_fd((char *)current_env->content, 1);
		write(1, "\n", 1);
		current_env = current_env->next;
	}
}

int	replace_env(t_shell *shell, char *env_value)
{
	t_list	*to_be_replaced;

	to_be_replaced = find_env(shell->env_list, env_value);
	if (!to_be_replaced)
	{
		to_be_replaced = ft_lstnew(ft_strdup(env_value));
		if (!to_be_replaced)
			return (MALLOC_FAIL);
		ft_lstadd_back(&shell->env_list, to_be_replaced);
	}
	else
	{
		free(to_be_replaced->content);
		to_be_replaced->content = env_value;
	}
	return (SUCCESS);
}

int	export(t_shell *shell, char **argv)
{
	int		i;

	i = 1;
	if (argv[i] == NULL )
		return (print_env_as_export(shell), 0);
	while (argv[i])
	{
		replace_env(shell, argv[i]);
		i++;
	}
	return (0);
}
