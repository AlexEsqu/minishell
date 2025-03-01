/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:58:45 by mkling            #+#    #+#             */
/*   Updated: 2025/02/27 10:19:58 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_blank_token(t_shell *shell, t_list **dest, char *input);
void	add_delimiter_token(t_shell *shell, t_list **dest, char *input);
void	add_operator_token(t_shell *shell, t_list **dest, char *input);

void	add_last_exit_code_token(t_shell *shell, t_list **dest, char *input)
{
	char	*word;
	t_token	*token;

	word = ft_itoa(shell->last_exit_code);
	if (!word)
		return (set_error(MALLOC_FAIL, shell));
	token = create_token(shell, WORD, input[shell->index], word);
	if (!token)
		return (set_error(MALLOC_FAIL, shell));
	ft_lstadd_back(dest, ft_lstnew(token));
	shell->index += 2;
}

void	add_nonvar_token(t_shell *shell, t_list **dest, char *input)
{
	int		len;
	char	*word;
	t_token	*token;

	len = 0;
	while (input[shell->index + len])
	{
		if (is_valid_variable(&input[shell->index + len]))
			break ;
		if (letter_is(BLANK, &input[shell->index + len]))
			break ;
		len++;
	}
	word = ft_calloc(sizeof(char), len + 1);
	if (!word)
		return (set_error(MALLOC_FAIL, shell));
	ft_strlcat(word, &input[shell->index], len + 1);
	token = create_token(shell, WORD, input[shell->index], word);
	if (!token)
		return (set_error(MALLOC_FAIL, shell));
	ft_lstadd_back(dest, ft_lstnew(token));
	shell->index += len;
}

void	add_variable_token(t_shell *shell, t_list **dest, char *input)
{
	t_token	*token;
	int		len;
	char	*word;

	len = 1;
	while (input[shell->index + len])
	{
		if (is_valid_variable(&input[shell->index + len]))
			break ;
		if (!ft_isalnum(input[shell->index + len])
			&& input[shell->index + len] != '_')
			break ;
		len++;
	}
	word = ft_calloc(sizeof(char), len + 1);
	if (!word)
		return (set_error(MALLOC_FAIL, shell));
	ft_strlcat(word, &input[shell->index], len + 1);
	token = create_token(shell, VARIABLE, input[shell->index], word);
	if (!token)
		return (set_error(MALLOC_FAIL, shell));
	ft_lstadd_back(dest, ft_lstnew(token));
	shell->index += len;
	token->lexem = VARIABLE;
}

int	tokenize_variables(t_shell *shell, t_list **dest, char *input)
{
	t_token	*token;

	shell->index = 0;
	token = create_token(shell, START, '\0', NULL);
	if (!token)
		return (set_error(MALLOC_FAIL, shell), MALLOC_FAIL);
	ft_lstadd_back(dest, ft_lstnew(token));
	while (shell->index < ft_strlen(input))
	{
		if (input[shell->index] == '$' && input[shell->index + 1] == '?')
			add_last_exit_code_token(shell, dest, input);
		else if (is_valid_variable(&input[shell->index]))
			add_variable_token(shell, dest, input);
		else if (letter_is(BLANK, &input[shell->index]))
			add_blank_token(shell, dest, input);
		else if (letter_is(DELIMITER, &input[shell->index]))
			add_delimiter_token(shell, dest, input);
		else
			add_nonvar_token(shell, dest, input);
	}
	token = create_token(shell, END, '\n', NULL);
	if (!token)
		return (set_error(MALLOC_FAIL, shell), MALLOC_FAIL);
	ft_lstadd_back(dest, ft_lstnew(token));
	return (SUCCESS);
}

t_list	*tokenize_and_expand_string(t_shell *shell, char *string)
{
	t_list	*token_list;
	t_list	*current;
	t_token	*token;

	token_list = NULL;
	tokenize_variables(shell, &token_list, string);
	current = token_list;
	while (current->next)
	{
		token = (t_token *)current->content;
		if (token->lexem == VARIABLE)
			expand_variable(shell, &token->content);
		current = current->next;
	}
	return (token_list);
}
