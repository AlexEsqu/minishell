/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 13:06:39 by alex              #+#    #+#             */
/*   Updated: 2025/02/14 11:36:45 by mkling           ###   ########.fr       */
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

t_tree	*parse_pipe(t_shell *shell, t_list **token)
{
	t_tree	*left;
	t_tree	*right;
	t_tree	*pipe_node;

	left = parse_command(shell, token);
	if (!left)
		return (NULL);
	if (!(*token) || !token_is(PIPE, (*token)))
		return (left);
	*token = (*token)->next;
	right = parse_pipe(shell, token);
	if (!right)
		return (free_tree(&left), NULL);
	pipe_node = create_branch(shell, AST_PIPE, NULL);
	pipe_node->left = left;
	pipe_node->right = right;
	return (pipe_node);
}

t_tree	*parse_and_or(t_shell *shell, t_list **node)
{
	t_tree	*left;
	t_tree	*right;
	t_tree	*pipe_node;
	int		type;

	left = parse_pipe(shell, node);
	if (!left)
		return (NULL);
	if (!(*node) || (!token_is(AND, (*node)) && !token_is(OR, (*node))))
		return (left);
	if (token_is(AND, (*node)))
		type = AST_AND;
	else
		type = AST_OR;
	*node = (*node)->next;
	right = parse_and_or(shell, node);
	if (!right)
		return (free_tree(&left), NULL);
	pipe_node = create_branch(shell, type, NULL);
	pipe_node->left = left;
	pipe_node->right = right;
	return (pipe_node);
}

void	parser(t_shell *shell)
{
	t_list	*current;

	current = shell->token_list;
	shell->tree_root = parse_and_or(shell, &current);
	ft_lstclear(&shell->token_list, free_token);
}
