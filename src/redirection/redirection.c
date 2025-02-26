/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 15:40:41 by vgodoy            #+#    #+#             */
/*   Updated: 2025/02/26 14:21:10 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*This function opens a file
    if : OUTFILE or APPEND -> output redirection
    else : -> input redirection*/
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

/*This function checks that file->path are
expanded, validated, and opened*/
void	expand_check_and_open(t_shell *shell, t_cmd *cmd, t_file *file)
{
	char	*stored_var;

	stored_var = ft_strdup(file->path);
	if (!stored_var)
		return (set_cmd_error(MALLOC_FAIL, cmd, NULL));
	expand_string(shell, &file->path);
	if (!file->path)
	{
		set_cmd_error(AMBIG_REDIR, cmd, stored_var);
		free(stored_var);
	}
	check_file(shell, cmd, file);
	if (cmd->exit_code)
		return ;
	open_file(cmd, file->mode, file->path);
	free(stored_var);
}

/*Checks file input/output redirections (<, >, >>):
	-expands and validates file paths
	-redirects standard input and output to the correct files
	-handles errors*/
void	redirect_for_cmd(t_shell *shell, t_cmd *cmd)
{
	t_list	*current;

	if (shell->critical_er || cmd->exit_code)
		return ;
	current = cmd->files;
	while (current)
	{
		expand_check_and_open(shell, cmd, (t_file *)current->content);
		if (cmd->exit_code)
			return ;
		current = current->next;
	}
	if (!cmd->arg_list)
		return ;
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

/*Closes files
if fd_in different from standard in, closes file and marks as -2
if fd_out different from standard out, closes file and marks as -2*/
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
