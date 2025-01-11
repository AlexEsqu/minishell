/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 14:12:32 by alex              #+#    #+#             */
/*   Updated: 2025/01/11 11:10:40 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_if_directory(t_cmd *cmd, char *path)
{
	int	fd;

	fd = open(path, __O_DIRECTORY | O_RDONLY);
	if (fd < 0)
		return (set_cmd_error(CANT_EXECUTE_CMD, cmd, "Is not a directory"));
	close(fd);
	return (set_cmd_error(CANT_EXECUTE_CMD, cmd, "Is a directory"));
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
			return (set_cmd_error(MALLOC_FAIL, cmd, "Failed to allocate path"));
		if (access(tested_path, F_OK | R_OK) == 0)
		{
			cmd->cmd_path = tested_path;
			free(cmd_stem);
			return ;
		}
		free(tested_path);
	}
	free(cmd_stem);
	return (set_cmd_error(CANT_FIND_CMD, cmd, "Command not found"));
}

/* Accepts input format ./[relative_dir]/[binary] or /[absolute_dir]/[binary]*/
void	check_absolute_or_relative_path(t_cmd *cmd)
{
	if (access(cmd->cmd_path, F_OK) == 0)
	{
		if (access(cmd->cmd_path, X_OK) != 0)
			return (set_cmd_error(CANT_EXECUTE_CMD, cmd, "Permission denied"));
	}
}

/* Opens up env to get $PATH, split it, and tries each until one is found */
void	check_environ_paths(t_shell *shell, t_cmd *cmd)
{
	char	*path_env;

	if (!find_env(shell->env_list, "PATH"))
		return (set_cmd_error(CANT_FIND_CMD, cmd, "No PATH variable found"));
	path_env = (char *)find_env(shell->env_list, "PATH")->content;
	shell->paths = ft_split(&path_env[5], ':');
	if (!shell->paths)
		set_error(MALLOC_FAIL, shell, "Failed to split path list");
	check_each_path(cmd, shell->paths);
	if (cmd->exit_code)
		return ;
	if (access(cmd->cmd_path, X_OK) != 0)
		set_cmd_error(CANT_EXECUTE_CMD, cmd, "Permission denied");
}

/* Checks first if a command path exist
Throws error if path is directory or trailed by /
Tries absolute and relative path for command
If non existent, tries paths from $PATH */
void	get_cmd_path(t_shell *shell, t_cmd *cmd)
{
	if (!cmd || !cmd->arg_list || !((char *)cmd->arg_list->content)[0])
		return (set_cmd_error(CANT_FIND_CMD, cmd, "Command not found"));
	cmd->cmd_path = (char *)cmd->arg_list->content;
	if (cmd->cmd_path[ft_strlen(cmd->cmd_path) - 1] == '/')
		check_if_directory(cmd, cmd->cmd_path);
	if (!cmd->exit_code)
		check_absolute_or_relative_path(cmd);
	if (!cmd->exit_code && access(cmd->cmd_path, F_OK))
		check_environ_paths(shell, cmd);
}
