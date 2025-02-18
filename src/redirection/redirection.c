/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 17:34:05 by mkling            #+#    #+#             */
/*   Updated: 2025/02/18 10:14:20 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	open_file(t_cmd *cmd, int mode, char *path)
{
	if (mode == OUTFILE || mode == APPEND)
	{
		if (cmd->fd_out != -2)
			close(cmd->fd_out);
		if (mode == APPEND)
			cmd->fd_out = open(path, O_RDWR | O_APPEND | O_CREAT, 0666);
		else
			cmd->fd_out = open(path, O_WRONLY | O_TRUNC | O_CREAT, 0666);
		if (cmd->fd_out < 0)
			return (set_cmd_error(OPEN_ERROR, cmd, path));
	}
	else
	{
		if (cmd->fd_in != -2)
			close(cmd->fd_in);
		cmd->fd_in = open(path, O_RDONLY);
		if (cmd->fd_in < 0)
			return (set_cmd_error(OPEN_ERROR, cmd, path));
	}
}

void	redirect_for_cmd(t_shell *shell, t_cmd *cmd)
{
	t_list	*current;
	t_file	*current_file;

	if (shell->critical_er || cmd->exit_code)
		return ;
	current = cmd->files;
	while (current)
	{
		current_file = (t_file *)current->content;
		expand_string(shell, &current_file->path);
		open_file(cmd, current_file->mode, current_file->path);
		current = current->next;
	}
	if (cmd->fd_in == -2)
		cmd->fd_in = STDIN_FILENO;
	else if (dup2(cmd->fd_in, STDIN_FILENO) == -1
		|| close(cmd->fd_in) != SUCCESS)
		return (set_cmd_error(DUP_ERROR, cmd, NULL));
	if (cmd->fd_out == -2)
		cmd->fd_out = STDOUT_FILENO;
	else if (dup2(cmd->fd_out, STDOUT_FILENO) == -1
		|| close(cmd->fd_out) != SUCCESS)
		return (set_cmd_error(DUP_ERROR, cmd, NULL));
}

void	close_cmd_fd(t_cmd *cmd)
{
	if (cmd->fd_in != STDIN_FILENO && cmd->fd_in > 0)
	{
		close(cmd->fd_in);
		cmd->fd_in = -2;
	}
	if (cmd->fd_out != STDOUT_FILENO && cmd->fd_out > 0)
	{
		close(cmd->fd_out);
		cmd->fd_out = -2;
	}
}
