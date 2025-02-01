/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 16:30:36 by mkling            #+#    #+#             */
/*   Updated: 2025/02/01 20:39:44 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cd(t_shell *shell, char *path)
{
	int		exit_code;
	char	*pwd;

	if (path == NULL)
		path = getenv("HOME");
	exit_code = chdir(path);
	if (exit_code != 0)
	{
		print_error();
		ft_putstr_fd("cd :", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
	}
	pwd = ft_strjoin("PWD=", path);
	if (!pwd)
		return (MALLOC_FAIL);
	replace_env(shell, pwd);
	return (exit_code);
}
// cd sans argument renvoie vers le home
