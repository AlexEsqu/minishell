/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 14:12:32 by alex              #+#    #+#             */
/*   Updated: 2025/02/24 16:06:22 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_or_should_be_directory(t_cmd *cmd, char *path)
{
	int	fd;

	fd = open(path, __O_DIRECTORY | O_RDONLY);
	if (fd < 0)
	{
		if (cmd->cmd_path[ft_strlen(cmd->cmd_path) - 1] == '/')
			return (set_cmd_error(IS_NOT_DIR, cmd, cmd->cmd_path), (1));
		else
			return (0);
	}
	close(fd);
	return (set_cmd_error(IS_DIR, cmd, cmd->cmd_path), 1);
}

static int	check_each_path(t_cmd *cmd, char **paths)
{
	char	*cmd_stem;
	char	*tested_path;
	size_t	i;

	cmd_stem = ft_strjoin("/", cmd->cmd_path);
	i = 0;
	while (paths[i])
	{
		free(cmd->cmd_path);
		cmd->cmd_path = NULL;
		tested_path = ft_strjoin(paths[i], cmd_stem);
		if (!tested_path)
			return (set_cmd_error(MALLOC_FAIL, cmd, cmd->arg_list->content), 0);
		if (access(tested_path, F_OK | R_OK) == 0)
		{
			cmd->cmd_path = tested_path;
			free(cmd_stem);
			return (1);
		}
		free(tested_path);
		i++;
	}
	free(cmd_stem);
	return (set_cmd_error(NO_CMD, cmd, cmd->arg_list->content), 0);
}

/* Accepts input format ./[relative_dir]/[binary] or /[absolute_dir]/[binary]*/
static int	is_in_absolute_or_relative_path(t_cmd *cmd)
{
	if (access(cmd->cmd_path, F_OK) == 0)
	{
		if (access(cmd->cmd_path, X_OK) != 0)
			set_cmd_error(PERM_ERROR, cmd, cmd->cmd_path);
		return (1);
	}
	return (0);
}

/* Opens up env to get $PATH or if non existant sets up a basic PATH variable
split it
Tries each path until one is found or return CMD NOT FOUND */
static int	is_in_environ_paths(t_shell *shell, t_cmd *cmd)
{
	t_list	*path_env;
	char	*path_content;
	int		is_in_environ;

	path_env = find_env(shell->env_list, "PATH");
	if (!path_env)
		path_content = "PATH=/bin";
	else
		path_content = (char *)path_env->content;
	shell->paths = ft_split(&path_content[5], ':');
	if (!shell->paths)
		set_error(MALLOC_FAIL, shell);
	is_in_environ = check_each_path(cmd, shell->paths);
	if (!cmd->exit_code && access(cmd->cmd_path, X_OK) != 0)
		return (set_cmd_error(PERM_ERROR, cmd, cmd->cmd_path), 1);
	ft_free_tab(shell->paths);
	shell->paths = NULL;
	return (is_in_environ);
}

/* Checks successively, according to bahs man 3.7.2 on GNU.org :

1. if cmd name has no slash:
	if it exists in the working directory, invokes it
	else if it is a builtin, invokes it
	else seeks it using the $PATH variable, invokes it
	else returns CMD NOT FOUND exit code 127

2. if search was successful OR cmd name contains slashes:
	executes it directly*/
void	find_cmd_path(t_shell *shell, t_cmd *cmd)
{
	if (cmd->exit_code)
		return ;
	if (!cmd || !cmd->arg_list || !cmd->arg_list->content)
		return (set_cmd_error(NO_CMD, cmd, NULL));
	cmd->cmd_path = ft_strdup((char *)cmd->arg_list->content);
	if (!ft_strchr(cmd->cmd_path, '/'))
	{
		if (is_in_absolute_or_relative_path(cmd))
			return ;
		else if (is_builtin(cmd))
			return ;
		else if (is_in_environ_paths(shell, cmd))
			return ;
	}
	else
	{
		if (is_or_should_be_directory(cmd, cmd->cmd_path))
			return ;
		if (is_in_absolute_or_relative_path(cmd))
			return ;
	}
	return (set_cmd_error(NO_CMD, cmd, NULL));
}
