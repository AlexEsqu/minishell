/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 16:37:27 by alex              #+#    #+#             */
/*   Updated: 2025/02/24 08:32:35 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_delimiter_token(t_shell *shell, t_list **dest, char *input)
{
	t_token	*token;
	char	*delim;

	delim = ft_calloc(sizeof(char), 2);
	ft_strlcat(delim, &input[shell->index], 2);
	if (!delim)
		return (set_error(MALLOC_FAIL, shell));
	if (delim[0] == '(')
		token = create_token(shell, OPEN_PARENTH, input[shell->index], delim);
	else if (delim[0] == ')')
		token = create_token(shell, CLOS_PARENTH, input[shell->index], delim);
	else
		token = create_token(shell, DELIMITER, input[shell->index], delim);
	if (!token)
		return (set_error(MALLOC_FAIL, shell));
	ft_lstadd_back(dest, ft_lstnew(token));
	shell->index++;
}

void	add_word_token(t_shell *shell, t_list **dest, char *input)
{
	int		len;
	char	*word;
	t_token	*token;

	len = 0;
	while (input[len])
	{
		if (!letter_is(WORD, &input[shell->index + len]))
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

void	add_blank_token(t_shell *shell, t_list **dest, char *input)
{
	int		len;
	char	*space;
	t_token	*token;

	len = 0;
	while (letter_is(BLANK, &input[shell->index + len]))
		len++;
	space = ft_calloc(sizeof(char), len + 1);
	if (!space)
		return (set_error(MALLOC_FAIL, shell));
	ft_strlcat(space, &input[shell->index], len + 1);
	token = create_token(shell, BLANK, input[shell->index], space);
	ft_lstadd_back(dest, ft_lstnew(token));
	shell->index += len;
}

void	add_operator_token(t_shell *shell, t_list **dest, char *input)
{
	t_token	*token;
	int		len;

	len = 1;
	if (input[shell->index] == input[shell->index + 1])
		len = 2;
	token = create_token(shell, OPERATOR, input[shell->index], NULL);
	token->content = ft_calloc(sizeof(char), len + 1);
	if (!token)
		return ;
	ft_strlcat(token->content, &input[shell->index], 2);
	if (!token->content)
		return (set_error(MALLOC_FAIL, shell));
	token->lexem = OPERATOR;
	ft_strlcat(token->content, &input[shell->index], len + 1);
	ft_lstadd_back(dest, ft_lstnew(token));
	shell->index += len;
}

void	scan(t_shell *shell, t_list **dest, char *input)
{
	shell->index = 0;
	ft_lstadd_back(dest, ft_lstnew(create_token(shell, START, '\0', NULL)));
	while (shell->index < ft_strlen(input))
	{
		if (letter_is(DELIMITER, &input[shell->index]))
			add_delimiter_token(shell, dest, input);
		else if (letter_is(OPERATOR, &input[shell->index]))
			add_operator_token(shell, dest, input);
		else if (letter_is(BLANK, &input[shell->index]))
			add_blank_token(shell, dest, input);
		else
			add_word_token(shell, dest, input);
	}
	ft_lstadd_back(dest, ft_lstnew(create_token(shell, END, '\n', NULL)));
	// print_tokens(*dest);
	apply_to_list(shell, *dest, id_escaped_char);
	apply_to_list(shell, *dest, group_strings);
	// print_tokens(*dest);
}
