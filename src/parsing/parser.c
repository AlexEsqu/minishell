/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 13:06:39 by alex              #+#    #+#             */
/*   Updated: 2024/12/28 19:21:09 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse_in_out_files(t_shell *shell, t_cmd *cmd, t_list **current)
{
	if (shell->critical_er)
		return ;
	if (token_is(APPEND, (*current)) || token_is(OUTFILE, (*current)))
	{
		if (token_is(WORD, (*current)->next))
			return (set_error(SYNTAX_ERROR, shell, "No outfile"));
		*current = (*current)->next;
		((t_token *)(*current)->content)->lexem = APPEND;
	}
	else if (token_is(HEREDOC, (*current)))
	{
		if (!token_is(WORD, (*current)->next))
			return (set_error(SYNTAX_ERROR, shell, "No delimiter"));
		*current = (*current)->next;
		((t_token *)(*current)->content)->lexem = HEREDOC;
	}
	else if (token_is(OUTFILE, (*current)))
	{
		if (!token_is(WORD, (*current)->next))
			return (set_error(SYNTAX_ERROR, shell, "No infile"));
		*current = (*current)->next;
		((t_token *)(*current)->content)->lexem = INFILE;
	}
	create_file(shell, cmd, ((t_token *)(*current)->content));
}

t_tree	*parse_command(t_shell *shell, t_list **node)
{
	t_cmd	*cmd;

	if (!(*node))
		return (set_error(CANT_FIND_CMD, shell, "Missing command"), NULL);
	cmd = create_cmd();
	while ((*node)->next && !token_is(PIPE, (*node)) && !token_is(END, (*node))
		&& !token_is(OR, (*node)) && !token_is(AND, (*node)))
	{
		if (token_is(OUTFILE, (*node)) || token_is(APPEND, (*node))
			|| token_is(INFILE, (*node)) || token_is(HEREDOC, (*node)))
			parse_in_out_files(shell, cmd, node);
		else if (token_is(WORD, (*node)) || token_is(STRING, (*node))
			|| token_is(VARIABLE, (*node)))
			ft_lstadd_back(&cmd->arg_list,
				ft_lstnew(ft_strdup(((t_token *)(*node)->content)->content)));
		*node = (*node)->next;
	}
	return (create_branch(shell, AST_CMD, cmd));
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
	shell->tree_root = parse_and_or(shell, &shell->token_list);
	ft_lstclear(&shell->token_list, free_token);
}
