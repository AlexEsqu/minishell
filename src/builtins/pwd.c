/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 19:56:20 by mkling            #+#    #+#             */
/*   Updated: 2025/02/24 14:06:29 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pwd(t_shell *shell, t_cmd *cmd)
{
	char	*pwd;
	t_list	*pwd_env;
	char	cwd_buffer[PATH_MAX + 1];

	pwd_env = find_env(shell->env_list, "PWD");
	if (pwd_env == NULL || pwd_env->content == NULL
		|| ft_strlen((char *)pwd_env->content) < 4)
	{
		if (getcwd(cwd_buffer, PATH_MAX) == NULL)
			return (set_cmd_error(NO_PATH, cmd, NULL), NO_PATH);
		ft_strlcat(cwd_buffer, "\n", PATH_MAX);
		ft_putstr_fd(cwd_buffer, STDOUT_FILENO);
		return (0);
	}
	pwd = (char *)pwd_env->content;
	ft_putstr_fd(&pwd[4], STDOUT_FILENO);
	write(STDOUT_FILENO, "\n", 1);
	return (0);
}
