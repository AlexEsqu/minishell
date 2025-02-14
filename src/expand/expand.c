/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 16:54:16 by mkling            #+#    #+#             */
/*   Updated: 2025/02/14 11:30:01 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_string(t_shell *shell, char **ptr_to_str);

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
			expanded_var = ft_strdup(relevant_env->content
					+ ft_strlen(var_name));
		else
			expanded_var = NULL;
	}
	free(var_name);
	*ptr_to_variable = expanded_var;
}

t_list	*tokenize_and_expand_string(t_shell *shell, char *string)
{
	t_list	*token_list;
	t_list	*current;
	t_token	*token;

	token_list = NULL;
	scan(shell, &token_list, string);
	apply_to_list(shell, token_list, id_variables);
	apply_to_list(shell, token_list, group_strings);
	current = token_list;
	while (current->next)
	{
		token = (t_token *)current->content;
		if (token->lexem == VARIABLE)
			expand_variable(shell, &token->content);
		if (token->lexem == STRING && token->letter != '\'')
			expand_string(shell, &token->content);
		current = current->next;
	}
	return (token_list);
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

void	expand_string(t_shell *shell, char **ptr_to_str)
{
	t_list	*token_list;
	char	*original_string;
	char	*expanded_string;

	if (!ptr_to_str || !(*ptr_to_str))
		return ;
	original_string = (*ptr_to_str);
	token_list = tokenize_and_expand_string(shell, original_string);
	expanded_string = flatten_token_list_into_string(shell, token_list);
	ft_lstclear(&token_list, free_token);
	free(original_string);
	*ptr_to_str = expanded_string;
}

void	expand_node(t_shell *shell, t_list *node)
{
	char	**ptr_to_str;

	if (!node || !node->content)
		return ;
	ptr_to_str = (char **)&node->content;
	if (is_valid_variable(node->content))
		expand_variable(shell, ptr_to_str);
	else if (has_valid_var(node->content) && can_expand(node))
	{
		remove_delimiter(shell, ptr_to_str);
		expand_string(shell, ptr_to_str);
	}
	else
		remove_delimiter(shell, ptr_to_str);
}
