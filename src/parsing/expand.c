/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 16:54:16 by mkling            #+#    #+#             */
/*   Updated: 2025/01/12 01:01:20 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_variable(t_shell *shell, char **ptr_to_variable)
{
	char	*var_name;
	char	*expanded_var;
	t_list	*relevant_env;

	var_name = (char *)(*ptr_to_variable);
	if (ft_strcmp(var_name, "$?") == 0)
		expanded_var = ft_itoa(shell->last_exit_code);
	else
	{
		relevant_env = find_env(shell->env_list, &var_name[1]);
		if (relevant_env)
			expanded_var = (char *)relevant_env->content + ft_strlen(var_name);
		else
			expanded_var = NULL;
	}
	free(var_name);
	*ptr_to_variable = expanded_var;
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

char	*flatten_token_list_into_string(t_shell *shell, t_list *head)
{
	int		len;
	t_list	*current;
	t_token	*token;
	char	*result;

	if (shell->critical_er)
		return (NULL);
	current = head;
	len = 0;
	while (current->next)
	{
		if (current->content && ((t_token *)current->content)->content)
			len += ft_strlen((char *)((t_token *)current->content)->content);
		current = current->next;
	}
	result = ft_calloc(sizeof(char), len + 1);
	current = head;
	while (current->next)
	{
		token = (t_token *)current->content;
		if (current->content && ((t_token *)current->content)->content)
			result = ft_strjoinfree(result, (char *)token->content);
		current = current->next;
	}
	return (result);
}

void	expand_in_string(t_shell *shell, t_list *node)
{
	t_list	*token_list;
	t_list	*current;
	t_token	*token;
	char	*expanded_string;

	if (!node || !node->content)
		return ;
	token_list = NULL;
	scan(shell, &token_list, (char *)node->content);
	apply_to_list(shell, token_list, id_variables);
	apply_to_list(shell, token_list, group_strings);
	current = token_list;
	while (current->next)
	{
		token = (t_token *)current->content;
		if (token->lexem == VARIABLE)
			expand_variable(shell, &token->content);
		current = current->next;
	}
	expanded_string = flatten_token_list_into_string(shell, token_list);
	free(node->content);
	node->content = expanded_string;
}

void	expand(t_shell *shell, t_list *node)
{
	if (!node || !node->content)
		return ;
	if (is_valid_variable((char *)node->content))
		expand_variable(shell, (char **)&node->content);
	if (has_valid_var((char *)node->content) && can_expand(node))
	{
		remove_delimiter(shell, &node->content);
		expand_in_string(shell, node);
	}
	else
		remove_delimiter(shell, &node->content);
}
