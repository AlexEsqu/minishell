/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 11:15:51 by mkling            #+#    #+#             */
/*   Updated: 2025/02/19 16:40:34 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_subshell(t_shell *shell, t_list **current)
{
	t_shell	*subshell;
	char	*cmd_line;
	int		exit_code;

	cmd_line = ft_strdup((char *)((t_token *)(*current)->content)->content);
	remove_parenthesis_from_string(shell, &cmd_line);
	printf("%s\n", cmd_line);
	subshell = create_minishell(shell->env);
	exit_code = 0;
	parse_and_exec_cmd(subshell, cmd_line);
	exit_code = subshell->last_exit_code;
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
