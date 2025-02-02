/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 19:56:20 by mkling            #+#    #+#             */
/*   Updated: 2025/02/02 12:56:26 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pwd(t_shell *shell, t_cmd *cmd)
{
	char	*pwd;
	t_list	*pwd_env;

	pwd_env = find_env(shell->env_list, "PWD");
	if (pwd_env == NULL || pwd_env->content == NULL
		|| ft_strlen((char *)pwd_env->content) < 4)
		return (set_cmd_error(PATH_ERROR, cmd, NULL), PATH_ERROR);
	pwd = (char *)pwd_env->content;
	ft_putstr_fd(&pwd[4], STDOUT_FILENO);
	write(STDOUT_FILENO, "\n", 1);
	return (0);
}
