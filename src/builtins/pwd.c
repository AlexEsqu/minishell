/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 18:10:18 by vgodoy            #+#    #+#             */
/*   Updated: 2025/02/26 13:38:20 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pwd(t_shell *shell, t_cmd *cmd)
{
	char	*pwd;
	t_list	*pwd_env;
	char	cwd_buffer[PATH_MAX + 1];

	pwd_env = find_env(shell, shell->env_list, "PWD");
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
