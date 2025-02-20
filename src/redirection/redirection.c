/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgodoy <vgodoy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 15:40:41 by vgodoy            #+#    #+#             */
/*   Updated: 2025/02/20 20:11:30 by vgodoy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


/*This function is responsible for opening a file for a command (t_cmd *cmd)
based on the specified mode (mode) and file path (path).
    if : OUTFILE or APPEND (indicating output redirection):
    else : Otherwise (input redirection):*/
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

/*if heredoc or error does nothing
INFILE : verify if there is a file / verify if we can access file
OUTFILE : if it exists and we connot write -> perm error*/
void	check_file(t_shell *shell, t_cmd *cmd, t_file *file)
{
	char	*expanded_filepath;

	if (file->mode == HEREDOC || cmd->exit_code)
		return ;
	expanded_filepath = ft_strdup(file->path);
	expand_string(shell, &expanded_filepath);
	if (file->mode == INFILE)
	{
		if (access(expanded_filepath, F_OK) != SUCCESS)
			set_cmd_error(NO_FILE, cmd, file->path);
		else if (access(expanded_filepath, R_OK) != SUCCESS)
			set_cmd_error(PERM_ERROR, cmd, file->path);
	}
	else if (file->mode == OUTFILE || file->mode == APPEND)
	{
		if (access(expanded_filepath, F_OK) == 0
			&& access(expanded_filepath, W_OK) != 0)
			set_cmd_error(PERM_ERROR, cmd, file->path);
	}
	free(expanded_filepath);
}

/*This function ensures that file paths are properly
expanded, validated, and opened,
handling potential errors like ambiguous redirections before proceeding.*/
void	expand_check_and_open(t_shell *shell, t_cmd *cmd, t_file *file)
{
	char	*stored_var;

	stored_var = ft_strdup(file->path);
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

/*Ensures file input/output redirection (<, >, >>) are processed correctly
before executing a command.
    Expands and validates file paths before opening them.
    Redirects standard input and output to the correct files.
    Handles errors properly (e.g., invalid files, failed dup2 calls).*/
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

/*Prevents file descriptor leaks by closing files
  when they are no longer needed.
Ensures that only opened, non-standard file descriptors are closed.
Marks closed file descriptors as -2, indicating they are invalid or unused.*/
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
