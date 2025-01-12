/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 09:19:30 by skassimi          #+#    #+#             */
/*   Updated: 2025/01/12 00:56:58 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_home(t_shell *shell)
{
	char	*path;

	while (shell->env_list)
	{
		path = (char *)shell->env_list->content;
		if (ft_strncmp(path, "HOME=", 5) == 0)
			return (&path[5]);
		shell->env_list = shell->env_list->next;
	}
	return (NULL);
}

int	cd(t_shell *shell, char *path)
{
	int	exit_code;

	if (path == NULL)
		path = extract_home(shell);
	exit_code = chdir(path);
	if (exit_code != 0)
	{
		print_error();
		ft_putstr_fd("cd :", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
	}
	return (exit_code);
}
// cd sans argument renvoie vers le home
