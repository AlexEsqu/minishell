/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 13:06:39 by alex              #+#    #+#             */
/*   Updated: 2025/01/30 17:33:30 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	token_is_redirection(t_list *token_node)
{
	return (token_is(OUTFILE, token_node) || token_is(APPEND, token_node)
		|| token_is(INFILE, token_node) || token_is(HEREDOC, token_node));
}

int	token_is_operator(t_list *token_node)
{
	return (token_is(PIPE, token_node) || token_is(END, token_node)
		|| token_is(OR, token_node) || token_is(AND, token_node));
}

void	parse_in_out_files(t_shell *shell, t_cmd *cmd, t_list **current)
{
	if (shell->critical_er || cmd->exit_code)
		return ;
	if (!token_is(WORD, (*current)->next))
		return (set_error(SYNTAX_ERROR, shell));
	*current = (*current)->next;
	((t_token *)(*current)->content)->lexem
		= ((t_token *)(*current)->prev->content)->lexem;
	open_file_and_store_fd_in_cmd(shell, cmd, *current);
}

t_tree	*parse_command(t_shell *shell, t_list **node)
{
	t_cmd	*cmd;
	char	*word;

	if (!(*node))
		return (set_error(CANT_FIND_CMD, shell), NULL);
	cmd = create_cmd();
	while ((*node)->next && !token_is_operator(*node))
	{
		if (token_is_redirection((*node)))
		{
			parse_in_out_files(shell, cmd, node);
			if (cmd->exit_code)
				return (NULL);
		}
		else if (token_is(WORD, (*node)) || token_is(STRING, (*node))
			|| token_is(VARIABLE, (*node)))
		{
			word = ft_strdup(((t_token *)(*node)->content)->content);
			if (!word)
				return (set_error(MALLOC_FAIL, shell), NULL);
			ft_lstadd_back(&cmd->arg_list, ft_lstnew(word));
		}
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
