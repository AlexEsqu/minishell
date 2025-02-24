/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verifications.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 16:17:43 by mkling            #+#    #+#             */
/*   Updated: 2025/02/24 16:53:37 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	check_if_invalid_infile(t_shell *shell, t_cmd *cmd, char *filepath)
{
	int	dir_fd;

	dir_fd = open(filepath, __O_DIRECTORY | O_RDONLY);
	if (dir_fd == 0)
	{
		close(dir_fd);
		return (set_cmd_error(IS_DIR, cmd, filepath));
	}
	if (filepath[ft_strlen(filepath) - 1] == '/')
	{
		if (access(filepath, F_OK) == 0)
			return (set_cmd_error(IS_NOT_DIR, cmd, filepath));
		else
			return (set_cmd_error(IS_DIR, cmd, filepath));
	}
	if (access(filepath, F_OK) != SUCCESS)
		return (set_cmd_error(NO_FILE, cmd, filepath));
	if (access(filepath, R_OK) != SUCCESS)
		return (set_cmd_error(PERM_ERROR, cmd, filepath));
}

static void	check_if_invalid_outfile(t_shell *shell, t_cmd *cmd, char *filepath)
{
	int	dir_fd;

	dir_fd = open(filepath, __O_DIRECTORY | O_RDONLY);
	if (dir_fd == 0)
	{
		close(dir_fd);
		return (set_cmd_error(IS_DIR, cmd, filepath));
	}
	if (filepath[ft_strlen(filepath) - 1] == '/')
		return (set_cmd_error(IS_DIR, cmd, filepath));
	if (access(filepath, F_OK) == SUCCESS)
	{
		if (access(filepath, W_OK) != 0)
			set_cmd_error(PERM_ERROR, cmd, filepath);
	}
}

void	check_file(t_shell *shell, t_cmd *cmd, t_file *file)
{
	char	*expanded_filepath;

	if (file->mode == HEREDOC || cmd->exit_code)
		return ;
	expanded_filepath = ft_strdup(file->path);
	if (!expanded_filepath)
		return (set_error(MALLOC_FAIL, shell));
	expand_string(shell, &expanded_filepath);
	if (!expanded_filepath)
		return ;
	if (file->mode == INFILE)
		check_if_invalid_infile(shell, cmd, expanded_filepath);
	else if (file->mode == OUTFILE || file->mode == APPEND)
		check_if_invalid_outfile(shell, cmd, expanded_filepath);
	free(expanded_filepath);
}
