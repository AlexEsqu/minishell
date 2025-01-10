/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 17:34:47 by alex              #+#    #+#             */
/*   Updated: 2024/12/30 13:37:49 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_space(t_shell *shell, t_list *current)
{
	if (shell->critical_er || !token_is(BLANK, current->next))
		return ;
	ft_lstpop(&shell->token_list, current->next, free_token);
}

/* For use on char, not on tokens */
void	remove_delimiter(t_shell *shell, void **ptr_to_string)
{
	char	*original_string;
	char	*quotefree_string;
	size_t	len;

	if (shell->critical_er || !ptr_to_string || !*ptr_to_string)
		return ;
	original_string = *ptr_to_string;
	len = ft_strlen(original_string);
	if (ft_strchr(DELIMITERS, original_string[0]) == NULL
		|| ft_strchr(DELIMITERS, original_string[len]) == NULL)
		return ;
	quotefree_string = ft_calloc(sizeof(char), len - 1);
	ft_strlcat(quotefree_string, &original_string[1], len - 1);
	free(original_string);
	*ptr_to_string = quotefree_string;
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
	// print_tokens(*token_list);
}

// TO DO
// escape character
// when to expand variables
// id variables

