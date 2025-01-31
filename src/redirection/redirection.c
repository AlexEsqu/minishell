/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 17:34:05 by mkling            #+#    #+#             */
/*   Updated: 2025/01/30 17:32:43 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	open_file(t_cmd *cmd, int mode, char *filepath)
{
	if (mode == OUTFILE || mode == APPEND)
	{
		if (access(filepath, W_OK) == -1)
			return (set_cmd_error(PERM_ERROR, cmd, filepath));
		if (mode == APPEND)
			cmd->fd_out = open(filepath, O_RDWR | O_APPEND | O_CREAT, 0666);
		else
			cmd->fd_out = open(filepath, O_WRONLY | O_TRUNC | O_CREAT, 0666);
	}
	if (mode == INFILE)
	{
		if (access(filepath, F_OK) == -1)
			return (set_cmd_error(NO_FILE, cmd, filepath));
		if (access(filepath, R_OK) == -1)
			return (set_cmd_error(READ_ERROR, cmd, filepath));
		cmd->fd_in = open(filepath, O_RDONLY);
	}
	if (cmd->fd_in == -1)
		return (set_cmd_error(OPEN_ERROR, cmd, filepath));
}

void	redirect_for_cmd(t_shell *shell, t_cmd *cmd)
{
	if (shell->critical_er || cmd->exit_code)
		return ;
	if (cmd->fd_in == -2)
		cmd->fd_in = STDIN_FILENO;
	else if (dup2(cmd->fd_in, STDIN_FILENO) == -1)
		return (set_cmd_error(DUP_ERROR, cmd, NULL));
	if (cmd->fd_out == -2)
		cmd->fd_out = STDOUT_FILENO;
	else if (dup2(cmd->fd_in, STDOUT_FILENO) == -1)
		return (set_cmd_error(DUP_ERROR, cmd, NULL));
}


void	open_file_and_store_fd_in_cmd(t_shell *shell, t_cmd *cmd, t_list *node)
{
	t_token	*token;

	token = ((t_token *)node->content);
	if (token->lexem == OUTFILE || token->lexem == APPEND)
	{
		if (cmd->fd_out != -2)
			close(cmd->fd_out);
		open_file(cmd, token->lexem, token->content);
	}
	if (token->lexem == INFILE || token->lexem == HEREDOC)
	{
		if (cmd->fd_in != -2)
			close(cmd->fd_in);
		if (token->lexem == HEREDOC)
			cmd->fd_in = assemble_heredoc(shell, cmd, token->content);
		else
			open_file(cmd, INFILE, token->content);
	}
}


