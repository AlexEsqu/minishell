/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:37:36 by mkling            #+#    #+#             */
/*   Updated: 2025/02/10 16:18:30 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_with_fork(t_shell *shell, t_cmd *cmd)
{
	if (cmd->exit_code)
		return (cmd->exit_code);
	if (create_fork(shell, &cmd->fork_pid) != SUCCESS)
		return (set_cmd_error(FORK_ERROR, cmd, NULL), FORK_ERROR);
	if (cmd->fork_pid == 0)
	{
		redirect_for_cmd(shell, cmd);
		find_cmd_path(shell, cmd);
		if (cmd->exit_code)
			exit(cmd->exit_code);
		apply_to_list(shell, cmd->arg_list, expand);
		put_arg_in_array(cmd);
		if (!cmd->argv || cmd->exit_code)
			exit(cmd->exit_code);
		execve(cmd->cmd_path, cmd->argv, shell->env);
		free_minishell(shell);
		exit(E_CMD_FAIL);
	}
	waitpid(cmd->fork_pid, &cmd->exit_code, 0);
	return (WEXITSTATUS(cmd->exit_code));
}

int	exec_with_main(t_shell *shell, t_cmd *cmd, bool piped)
{
	apply_to_list(shell, cmd->arg_list, expand);
	put_arg_in_array(cmd);
	if (!cmd->argv)
		return (cmd->exit_code);
	redirect_for_cmd(shell, cmd);
	if (cmd->exit_code)
		return (cmd->exit_code);
	exec_builtin(shell, cmd);
	reset_std(shell, piped);
	return (WEXITSTATUS(cmd->exit_code));
}

int	exec_single_cmd(t_shell *shell, t_tree *tree, bool piped)
{
	t_cmd	*cmd;
	int		exit_code;

	cmd = (t_cmd *)tree->content;
	if (!cmd->arg_list || cmd->exit_code)
		exit_code = cmd->exit_code;
	else if (is_builtin(cmd))
		exit_code = exec_with_main(shell, cmd, piped);
	else
		exit_code = exec_with_fork(shell, cmd);
	free_cmd(cmd);
	return (exit_code);
}

int	exec_pipe(t_shell *shell, t_tree *tree)
{
	int	pipe_fd[2];
	int	fork_pid[2];
	int	exit_code;

	if (create_pipe(shell, pipe_fd) != SUCCESS)
		return (set_error(PIPE_ERROR, shell), PIPE_ERROR);
	if (create_fork(shell, &fork_pid[0]) != SUCCESS)
		return (set_error(FORK_ERROR, shell), FORK_ERROR);
	if (fork_pid[0] == 0)
		connect_pipes_and_exec(shell, tree->left, pipe_fd, WRITE);
	else
	{
		if (create_fork(shell, &fork_pid[1]) != SUCCESS)
			return (set_error(FORK_ERROR, shell), FORK_ERROR);
		if (fork_pid[1] == 0)
			connect_pipes_and_exec(shell, tree->right, pipe_fd, READ);
		else
		{
			close_pipe(pipe_fd);
			waitpid(fork_pid[0], &exit_code, 0);
			waitpid(fork_pid[1], &exit_code, 0);
			return (WEXITSTATUS(exit_code));
		}
	}
	return (PIPE_ERROR);
}

int	exec_tree(t_shell *shell, t_tree *tree, bool piped)
{
	int	exit_code;

	if (!tree)
		return (set_error(AST_ERROR, shell), AST_ERROR);
	if (tree->type == AST_PIPE)
		return (exec_pipe(shell, tree));
	if (tree->type == AST_AND)
	{
		exit_code = exec_tree(shell, tree->left, NO_PIPE);
		if (exit_code == SUCCESS)
			return (exec_tree(shell, tree->right, NO_PIPE));
		return (exit_code);
	}
	if (tree->type == AST_OR)
	{
		exit_code = exec_tree(shell, tree->left, NO_PIPE);
		if (exit_code != SUCCESS)
			return (exec_tree(shell, tree->right, NO_PIPE));
		return (exit_code);
	}
	return (exec_single_cmd(shell, tree, piped));
}
