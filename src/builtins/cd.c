/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 16:30:36 by mkling            #+#    #+#             */
/*   Updated: 2025/02/16 09:12:03 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	update_cwd(t_shell *shell, t_cmd *cmd)
{
	t_list	*env_pwd;
	char	cwd_buffer[PATH_MAX];
	char	*old_pwd;
	char	*pwd;

	if (getcwd(cwd_buffer, PATH_MAX) == NULL)
		return (set_cmd_error(PATH_ERROR, cmd, NULL), PATH_ERROR);
	pwd = ft_strjoin("PWD=", cwd_buffer);
	if (!pwd)
		return (set_cmd_error(MALLOC_FAIL, cmd, NULL), MALLOC_FAIL);
	env_pwd = find_env(shell->env_list, "PWD");
	if (!env_pwd)
		ft_lstadd_back(&shell->env_list, ft_lstnew(pwd));
	else
	{
		old_pwd = ft_strjoin("OLDPWD=", ((char *)env_pwd->content) + 4);
		if (!old_pwd)
			return (set_cmd_error(MALLOC_FAIL, cmd, NULL), MALLOC_FAIL);
		free(env_pwd->content);
		env_pwd->content = pwd;
		replace_env(shell, old_pwd);
	}
	return (SUCCESS);
}

int	cd(t_shell *shell, t_cmd *cmd)
{
	char	*path;
	t_list	*env_home;

	if (cmd->argv[1] && cmd->argv[2])
		return (set_cmd_error(TOO_MANY_ARGS, cmd, NULL), TOO_MANY_ARGS);
	if (cmd->argv[1] == NULL)
	{
		env_home = find_env(shell->env_list, "HOME");
		if (env_home && env_home->content && ft_strlen(env_home->content) > 5)
			path = env_home->content + 5;
	}
	else
		path = (char *)cmd->argv[1];
	if (path == NULL)
		return (set_cmd_error(E_SYNTAX, cmd, path), E_SYNTAX);
	cmd->exit_code = chdir(path);
	if (cmd->exit_code)
		return (set_cmd_error(NO_FILE, cmd, path), NO_FILE);
	update_cwd(shell, cmd);
	return (cmd->exit_code);
}
