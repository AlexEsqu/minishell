/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arguments.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 21:51:24 by mkling            #+#    #+#             */
/*   Updated: 2025/02/18 15:07:02 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tree	*create_branch(t_shell *shell, int type, void *content)
{
	t_tree	*node;

	node = ft_calloc(1, sizeof(t_tree));
	if (!node)
		return (set_error(MALLOC_FAIL, shell), NULL);
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
		return (set_error(MALLOC_FAIL, shell), NULL);
	index = 0;
	current = head;
	while (index < len)
	{
		result[index] = ft_strdup(current->content);
		current = current->next;
		index++;
	}
	return (result);
}

void	put_arg_in_array(t_cmd *cmd)
{
	int		index;
	t_list	*current;

	cmd->argc = ft_lstsize(cmd->arg_list);
	cmd->argv = ft_calloc(sizeof(char *), cmd->argc + 1);
	if (!cmd->argv)
		return (set_cmd_error(MALLOC_FAIL, cmd, NULL));
	index = 0;
	current = cmd->arg_list;
	while (index < cmd->argc)
	{
		cmd->argv[index] = current->content;
		current = current->next;
		index++;
	}
}
