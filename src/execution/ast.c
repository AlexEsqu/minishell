/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 11:15:51 by mkling            #+#    #+#             */
/*   Updated: 2025/02/26 14:00:11 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_subshell(t_shell *shell, char *input)
{
	t_shell	*subshell;
	char	*cmd_line;
	int		exit_code;

	cmd_line = ft_strdup(input);
	if (!cmd_line)
		return (E_CMD_FAIL);
	remove_parenthesis_from_string(shell, &cmd_line);
	subshell = create_minishell(shell->env);
	if (!subshell)
		return (E_CMD_FAIL);
	subshell->last_exit_code = 0;
	parse_and_exec_cmd(subshell, cmd_line);
	exit_code = subshell->last_exit_code;
	free_minishell(subshell);
	free(cmd_line);
	return (exit_code);
}

int	exec_tree(t_shell *shell, t_tree *tree, bool piped)
{
	int	exit_code;

	if (!tree)
	{
		exit_code = shell->critical_er;
		free_minishell(shell);
		return (exit_code);
	}
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
