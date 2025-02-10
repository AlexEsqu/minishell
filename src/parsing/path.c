/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 14:12:32 by alex              #+#    #+#             */
/*   Updated: 2025/02/10 17:01:18 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_if_directory(t_cmd *cmd, char *path)
{
	int	fd;

	fd = open(path, __O_DIRECTORY | O_RDONLY);
	if (fd < 0)
	{
		if (cmd->cmd_path[ft_strlen(cmd->cmd_path) - 1] == '/')
			return (set_cmd_error(IS_NOT_DIR, cmd, cmd->cmd_path));
		else
			return ;
	}
	close(fd);
	return (set_cmd_error(IS_DIR, cmd, cmd->cmd_path));
}

void	check_each_path(t_cmd *cmd, char **paths)
{
	char	*cmd_stem;
	char	*tested_path;
	size_t	i;

	cmd_stem = ft_strjoin("/", cmd->cmd_path);
	i = 0;
	while (paths[i])
	{
		tested_path = ft_strjoin(paths[i++], cmd_stem);
		if (!tested_path)
			return (set_cmd_error(MALLOC_FAIL, cmd, cmd->cmd_path));
		if (access(tested_path, F_OK | R_OK) == 0)
		{
			cmd->cmd_path = tested_path;
			free(cmd_stem);
			return ;
		}
		free(tested_path);
	}
	free(cmd_stem);
	return (set_cmd_error(NO_CMD, cmd, cmd->cmd_path));
}

/* Accepts input format ./[relative_dir]/[binary] or /[absolute_dir]/[binary]*/
void	check_absolute_or_relative_path(t_cmd *cmd)
{
	if (access(cmd->cmd_path, F_OK) == 0)
	{
		if (access(cmd->cmd_path, X_OK) != 0)
			return (set_cmd_error(PERM_ERROR, cmd, cmd->cmd_path));
	}
}

/* Opens up env to get $PATH, split it, and tries each until one is found */
void	check_environ_paths(t_shell *shell, t_cmd *cmd)
{
	char	*path_env;

	if (!find_env(shell->env_list, "PATH"))
		return (set_cmd_error(NO_CMD, cmd, cmd->cmd_path));
	path_env = (char *)find_env(shell->env_list, "PATH")->content;
	shell->paths = ft_split(&path_env[5], ':');
	if (!shell->paths)
		set_error(MALLOC_FAIL, shell);
	check_each_path(cmd, shell->paths);
	if (cmd->exit_code)
	{
		ft_free_tab(shell->paths);
		shell->paths = NULL;
		return ;
	}
	if (access(cmd->cmd_path, X_OK) != 0)
		set_cmd_error(PERM_ERROR, cmd, cmd->cmd_path);
}

/* Checks first if a command path exist
Throws error if path is directory or not dir but trailed by /
Tries absolute and relative path for command
If non existent, tries paths from $PATH */
void	find_cmd_path(t_shell *shell, t_cmd *cmd)
{
	if (cmd->exit_code)
		return ;
	if (!cmd || !cmd->arg_list || !((char *)cmd->arg_list->content)[0])
		return (set_cmd_error(NO_CMD, cmd, NULL));
	cmd->cmd_path = ft_strdup((char *)cmd->arg_list->content);
	check_if_directory(cmd, cmd->cmd_path);
	if (!cmd->exit_code)
		check_absolute_or_relative_path(cmd);
	if (!cmd->exit_code && access(cmd->cmd_path, F_OK))
		check_environ_paths(shell, cmd);
}
