/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:58:45 by mkling            #+#    #+#             */
/*   Updated: 2025/02/24 13:28:49 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_blank_token(t_shell *shell, t_list **dest, char *input);
void	add_delimiter_token(t_shell *shell, t_list **dest, char *input);
void	add_operator_token(t_shell *shell, t_list **dest, char *input);

void	add_alphanum_token(t_shell *shell, t_list **dest, char *input)
{
	int		len;
	char	*word;
	t_token	*token;

	len = 0;
	while (input[len])
	{
		if (is_valid_variable(&input[shell->index + len]))
			break ;
		else if (!letter_is(WORD, &input[shell->index + len]))
			break ;
		len++;
	}
	word = ft_calloc(sizeof(char), len + 1);
	if (!word)
		return (set_error(MALLOC_FAIL, shell));
	ft_strlcat(word, &input[shell->index], len + 1);
	token = create_token(shell, WORD, input[shell->index], word);
	ft_lstadd_back(dest, ft_lstnew(token));
	shell->index += len;
}

void	add_dollar_token(t_shell *shell, t_list **dest, char *input)
{
	t_token	*token;

	token = create_token(shell, DOLLAR, input[shell->index], NULL);
	token->content = ft_strdup("");
	token->lexem = DOLLAR;
	ft_lstadd_back(dest, ft_lstnew(token));
	shell->index += 1;
}

int	tokenize_variables(t_shell *shell, t_list **dest, char *input)
{
	shell->index = 0;
	ft_lstadd_back(dest, ft_lstnew(create_token(shell, START, '\0', NULL)));
	while (shell->index < ft_strlen(input))
	{
		if (is_valid_variable(&input[shell->index]))
			add_dollar_token(shell, dest, input);
		else if (letter_is(BLANK, &input[shell->index]))
			add_blank_token(shell, dest, input);
		else if (letter_is(DELIMITER, &input[shell->index]))
			add_delimiter_token(shell, dest, input);
		else
			add_alphanum_token(shell, dest, input);
	}
	ft_lstadd_back(dest, ft_lstnew(create_token(shell, END, '\n', NULL)));
	return (SUCCESS);
}

t_list	*tokenize_and_expand_string(t_shell *shell, char *string)
{
	t_list	*token_list;
	t_list	*current;
	t_token	*token;

	token_list = NULL;
	tokenize_variables(shell, &token_list, string);
	// apply_to_list(shell, token_list, group_strings);
	apply_to_list(shell, token_list, id_variables);
	current = token_list;
	while (current->next)
	{
		token = (t_token *)current->content;
		if (token->lexem == VARIABLE)
			expand_variable(shell, &token->content);
		// if (token->lexem == STRING && token->letter != '\'')
		// 	expand_string(shell, &token->content);
		current = current->next;
	}
	return (token_list);
}
