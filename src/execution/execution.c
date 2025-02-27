/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgodoy <vgodoy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:37:36 by mkling            #+#    #+#             */
/*   Updated: 2025/02/27 12:05:52 by vgodoy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	reset_std(t_shell *shell, bool piped)
{
	if (piped)
		return ;
	dup2(shell->std_in, 0);
	dup2(shell->std_out, 1);
}

static int	exec_binary(t_shell *shell, t_cmd *cmd)
{
	redirect_for_cmd(shell, cmd);
	if (cmd->exit_code)
		exit(cmd->exit_code);
	apply_to_list(shell, cmd->arg_list, expand_node);
	put_arg_in_array(cmd);
	if (!cmd->argv || !cmd->argv[0] || cmd->exit_code)
		exit(cmd->exit_code);
	execve(cmd->cmd_path, cmd->argv, shell->env);
	set_cmd_error(NO_FILE, cmd, NULL);
	free_minishell(shell);
	exit(E_NO_CMD);
}

static int	exec_for_builtin(t_shell *shell, t_cmd *cmd, bool piped)
{
	int	exit_code;

	redirect_for_cmd(shell, cmd);
	if (cmd->exit_code)
		return (cmd->exit_code);
	apply_to_list(shell, cmd->arg_list, expand_node);
	put_arg_in_array(cmd);
	if (!cmd->argv)
		return (cmd->exit_code);
	exit_code = exec_builtin(shell, cmd);
	reset_std(shell, piped);
	if (piped)
	{
		free_minishell(shell);
		exit(exit_code);
	}
	return (exit_code);
}

static int	create_fork_to_exec_binary(t_shell *shell, t_cmd *cmd)
{
	int	fork_pid;
	int	exit_code;

	exit_code = 0;
	if (create_fork(shell, &fork_pid))
		return (FORK_ERROR);
	if (fork_pid == 0)
		exec_binary(shell, cmd);
	else
		waitpid(fork_pid, &exit_code, 0);
	cmd->exit_code = WEXITSTATUS(exit_code);
	return (cmd->exit_code);
}

int	exec_single_cmd(t_shell *shell, t_tree *tree, bool piped)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)tree->content;
	if (cmd->exit_code)
		return (cmd->exit_code);
	if (!cmd->arg_list && cmd->files)
	{
		redirect_for_cmd(shell, cmd);
		reset_std(shell, piped);
		close_cmd_fd(cmd);
	}
	else if (is_builtin(cmd))
		cmd->exit_code = exec_for_builtin(shell, cmd, piped);
	else if (!piped)
		cmd->exit_code = create_fork_to_exec_binary(shell, cmd);
	else
		cmd->exit_code = exec_binary(shell, cmd);
	return (cmd->exit_code);
}
