/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgodoy <vgodoy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 11:15:51 by mkling            #+#    #+#             */
/*   Updated: 2025/02/14 16:51:57 by vgodoy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
