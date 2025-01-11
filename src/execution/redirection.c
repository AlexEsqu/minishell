/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 17:34:05 by mkling            #+#    #+#             */
/*   Updated: 2025/01/12 00:52:43 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	open_file(t_file *file, t_cmd *cmd, int mode)
{
	file->fd = 0;
	if (access(file->path, F_OK) == -1)
		return (set_cmd_error(NO_FILE, cmd, "No file"), NO_FILE);
	if (access(file->path, R_OK) == -1)
		return (set_cmd_error(READ_ERROR, cmd, "Forbidden file"), READ_ERROR);
	if (mode == READ)
		file->fd = open(file->path, O_RDONLY);
	if (mode == WRITE)
		file->fd = open(file->path, O_WRONLY | O_TRUNC | O_CREAT, 0666);
	if (mode == APPEND)
		file->fd = open(file->path, O_RDWR | O_APPEND | O_CREAT, 0666);
	if (file->fd == -1)
		return (set_cmd_error(OPEN_ERROR, cmd, "Error while opening file"), 1);
	return (SUCCESS);
}

void	set_infile_fd(t_shell *shell, t_cmd *cmd)
{
	t_file	*file;

	if (cmd->infiles == NULL)
	{
		cmd->fd_in = STDIN_FILENO;
		return ;
	}
	while (cmd->infiles)
	{
		file = (t_file *)cmd->infiles->content;
		if (file->mode == HEREDOC)
			assemble_heredoc(shell, cmd, cmd->infiles);
		else
			open_file(file, cmd, READ);
		if (cmd->exit_code)
			return ;
		if (cmd->infiles->next)
			close(file->fd);
		cmd->infiles = cmd->infiles->next;
	}
	cmd->fd_in = file->fd;
}

void	set_outfile_fd(t_cmd *cmd)
{
	t_file	*file;

	if (cmd->outfiles == NULL)
	{
		cmd->fd_out = STDOUT_FILENO;
		return ;
	}
	while (cmd->outfiles)
	{
		file = (t_file *)cmd->outfiles->content;
		open_file(file, cmd, file->mode);
		if (cmd->exit_code)
			return ;
		if (cmd->outfiles->next)
			close(file->fd);
		cmd->outfiles = cmd->outfiles->next;
	}
	cmd->fd_out = file->fd;
}

int	connect_pipes_and_exec(t_shell *shell, t_tree *tree, int pipe_fd[2], int mode)
{
	int	exit_code;

	if (mode == WRITE)
	{
		close(pipe_fd[READ]);
		dup2(pipe_fd[WRITE], STDOUT_FILENO);
		close(pipe_fd[WRITE]);
	}
	if (mode == READ)
	{
		close(pipe_fd[WRITE]);
		dup2(pipe_fd[READ], STDIN_FILENO);
		close(pipe_fd[READ]);
	}
	exit_code = exec_tree(shell, tree, true);
	exit (exit_code);
}

void	redirect_for_cmd(t_shell *shell, t_cmd *cmd)
{
	if (shell->critical_er)
		return ;
	set_infile_fd(shell, cmd);
	set_outfile_fd(cmd);
	if (cmd->exit_code)
		return ;
	if (cmd->fd_in != STDIN_FILENO && dup2(cmd->fd_in, STDIN_FILENO) == -1)
		return (set_cmd_error(DUP_ERROR, cmd, "Error redirecting input"));
	if (cmd->fd_out != STDOUT_FILENO && dup2(cmd->fd_in, STDOUT_FILENO) == -1)
		return (set_cmd_error(DUP_ERROR, cmd, "Error redirecting output"));
}
