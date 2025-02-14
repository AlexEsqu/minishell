/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:37:36 by mkling            #+#    #+#             */
/*   Updated: 2025/02/14 00:37:40 by alex             ###   ########.fr       */
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
	if (!cmd->argv || cmd->exit_code)
		exit(cmd->exit_code);
	execve(cmd->cmd_path, cmd->argv, shell->env);
	free_cmd(cmd);
	free_minishell(shell);
	exit(E_CMD_FAIL);
}

static int	exec_for_builtin(t_shell *shell, t_cmd *cmd, bool piped)
{
	int	exit_code;

	apply_to_list(shell, cmd->arg_list, expand_node);
	put_arg_in_array(cmd);
	if (!cmd->argv)
		return (cmd->exit_code);
	redirect_for_cmd(shell, cmd);
	if (cmd->exit_code)
		return (cmd->exit_code);
	exit_code = exec_builtin(shell, cmd);
	reset_std(shell, piped);
	return (WEXITSTATUS(exit_code));
}

static int	exec_single_cmd(t_shell *shell, t_tree *tree, bool piped)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)tree->content;
	if (cmd->exit_code)
		return (cmd->exit_code);
	if (!cmd->arg_list && cmd->files)
	{
		redirect_for_cmd(shell, cmd);
		reset_std(shell, piped);
	}
	else if (is_builtin(cmd))
		cmd->exit_code = exec_for_builtin(shell, cmd, piped);
	else
		cmd->exit_code = exec_binary(shell, cmd);
	return (cmd->exit_code);
}

int	exec_tree(t_shell *shell, t_tree *tree, bool piped)
{
	int	exit_code;

	if (!tree)
		return (set_error(AST_ERROR, shell), AST_ERROR);
	if (tree->type == AST_PIPE)
		return (exec_pipe_monitor(shell, tree));
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
