/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 17:34:47 by alex              #+#    #+#             */
/*   Updated: 2025/01/12 01:00:52 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_space(t_shell *shell, t_list *current)
{
	if (shell->critical_er || !token_is(BLANK, current->next))
		return ;
	ft_lstpop(&shell->token_list, current->next, free_token);
}

/* might want to add string grouping when word next to empty quotes */
void	group_strings(t_shell *shell, t_list *node)
{
	t_token	*first_delim;

	first_delim = ((t_token *)node->content);
	if (first_delim->lexem != DELIMITER)
		return ;
	while (!token_is(END, node->next))
	{
		if (((t_token *)node->next->content)->letter == first_delim->letter)
		{
			merge_token(shell, node);
			break ;
		}
		merge_token(shell, node);
	}
	first_delim->lexem = STRING;
}

void	id_variables(t_shell *shell, t_list *current)
{
	t_token	*token;

	token = (t_token *)current->content;
	if (shell->critical_er || token->letter != '$')
		return ;
	if (current->next->next)
		merge_token(shell, current);
	if (is_valid_variable((char *)((t_token *)current->content)->content))
		token->lexem = VARIABLE;
	else
		token->lexem = WORD;
}

void	id_operators(t_shell *shell, t_list *current)
{
	t_token	*token;

	if (shell->critical_er || !current || !token_is(OPERATOR, current))
		return ;
	token = (t_token *)current->content;
	if (ft_strcmp(token->content, "||") == 0)
		token->lexem = OR;
	else if (ft_strcmp(token->content, "&&") == 0)
		token->lexem = AND;
	else if (ft_strcmp(token->content, ">>") == 0)
		token->lexem = APPEND;
	else if (ft_strcmp(token->content, "<<") == 0)
		token->lexem = HEREDOC;
	else if (ft_strcmp(token->content, ">") == 0)
		token->lexem = OUTFILE;
	else if (ft_strcmp(token->content, "<") == 0)
		token->lexem = INFILE;
	else if (ft_strcmp(token->content, "|") == 0)
		token->lexem = PIPE;
	else if (ft_strcmp(token->content, "$") == 0)
		id_variables(shell, current);
	else
		token->lexem = WORD;
}

void	lexer(t_shell *shell, t_list **token_list)
{
	apply_to_list(shell, *token_list, id_operators);
	apply_to_list(shell, *token_list, group_strings);
	apply_to_list(shell, *token_list, remove_space);
}

// TO DO
// escape character
// when to expand variables
// id variables
