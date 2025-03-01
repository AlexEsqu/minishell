/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgodoy <vgodoy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 16:13:38 by mkling            #+#    #+#             */
/*   Updated: 2025/02/25 23:17:15 by vgodoy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	parse_in_out_files(t_shell *shell, t_cmd *cmd, t_list **current)
{
	if (shell->critical_er || cmd->exit_code)
		return ;
	*current = (*current)->next;
	((t_token *)(*current)->content)->lexem
		= ((t_token *)(*current)->prev->content)->lexem;
	create_file(shell, cmd, (t_token *)(*current)->content);
}

static void	parse_command_arg(t_shell *shell, t_cmd *cmd, t_list **current)
{
	char	*arg;

	arg = ft_strdup(((t_token *)(*current)->content)->content);
	if (!arg)
		return (set_error(MALLOC_FAIL, shell));
	ft_lstadd_back(&cmd->arg_list, ft_lstnew(arg));
	if (cmd->arg_list->next == NULL && arg[0] != '(')
		find_cmd_path(shell, cmd);
}

t_tree	*parse_command(t_shell *shell, t_list **node)
{
	t_cmd	*cmd;

	if (!(*node))
		return (NULL);
	cmd = create_cmd();
	while ((*node)->next && !token_is_operator(*node))
	{
		if (token_is_redirection((*node)))
			parse_in_out_files(shell, cmd, node);
		if (token_is(WORD, *node) || token_is(STRING, *node)
			|| token_is(VARIABLE, *node) || token_is(SUBSHELL, *node))
			parse_command_arg(shell, cmd, node);
		if (g_my_sig == CONTROL_C)
			return (NULL);
		*node = (*node)->next;
	}
	return (create_branch(shell, AST_CMD, cmd));
}
