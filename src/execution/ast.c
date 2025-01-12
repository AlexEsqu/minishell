/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 21:51:24 by mkling            #+#    #+#             */
/*   Updated: 2025/01/12 00:59:03 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tree	*create_branch(t_shell *shell, int type, void *content)
{
	t_tree	*node;

	node = ft_calloc(1, sizeof(t_tree));
	if (!node)
		return (set_error(MALLOC_FAIL, shell, "Failed to malloc tree"), NULL);
	node->type = type;
	node->content = content;
	node->left = NULL;
	node->left = NULL;
	return (node);
}

char	**extract_list_as_array(t_shell *shell, t_list *head)
{
	int		len;
	int		index;
	char	**result;
	t_list	*current;

	len = ft_lstsize(head);
	result = ft_calloc(sizeof(char *), len + 1);
	if (!result)
		return (set_error(MALLOC_FAIL, shell, "Failed to malloc argv"), NULL);
	index = 0;
	current = head;
	while (index < len)
	{
		result[index] = current->content;
		current = current->next;
		index++;
	}
	return (result);
}

void	put_arg_in_array(t_cmd *cmd)
{
	int	argc;
	int	index;

	argc = ft_lstsize(cmd->arg_list);
	cmd->argv = ft_calloc(sizeof(char *), argc + 1);
	if (!cmd->argv)
		return (set_cmd_error(MALLOC_FAIL, cmd, "Failed to malloc argv"));
	index = 0;
	while (index < argc)
	{
		cmd->argv[index] = cmd->arg_list->content;
		cmd->arg_list = cmd->arg_list->next;
		index++;
	}
}
