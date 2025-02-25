/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 09:52:49 by mkling            #+#    #+#             */
/*   Updated: 2025/02/25 11:43:50 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_backup_environ(t_shell *shell)
{
	t_list	*pwd_env;
	t_list	*path_env;
	char	buffer[PATH_MAX];

	ft_bzero(buffer, PATH_MAX);
	pwd_env = find_env(shell->env_list, "PWD");
	if (!pwd_env || ft_strlen(pwd_env->content) < 5)
	{
		ft_strlcat(buffer, "PWD=", 5);
		if (getcwd(&buffer[4], PATH_MAX) == NULL)
			return ;
		replace_env(shell, buffer);
	}
}

void	extract_env_as_linked_list(t_shell *shell)
{
	int		index;

	index = 0;
	while (shell->env[index])
	{
		ft_lstadd_back(&shell->env_list,
			ft_lstnew(ft_strdup(shell->env[index])));
		index++;
	}
	set_backup_environ(shell);
}

int	env(t_shell *shell)
{
	t_list	*current;

	current = shell->env_list;
	while (current != NULL)
	{
		ft_putstr_fd((char *)current->content, STDOUT_FILENO);
		write(1, "\n", STDOUT_FILENO);
		current = current->next;
	}
	return (0);
}
