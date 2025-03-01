/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 23:19:39 by vgodoy            #+#    #+#             */
/*   Updated: 2025/02/26 13:38:27 by alex             ###   ########.fr       */
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
		relevant_env = find_env(shell, shell->env_list, &var_name[1]);
		if (relevant_env)
			expanded_var = extract_env_value(shell, relevant_env->content);
		else
			expanded_var = NULL;
	}
	free(var_name);
	*ptr_to_variable = expanded_var;
}

int	ft_arraylen(t_list *head)
{
	t_list	*current;
	int		len;

	current = head;
	len = 0;
	while (current->next)
	{
		if (current->content && ((t_token *)current->content)->content)
			len += ft_strlen((char *)((t_token *)current->content)->content);
		current = current->next;
	}
	return (len);
}

char	*flatten_token_list_into_string(t_shell *shell, t_list *head)
{
	t_list	*current;
	t_token	*token;
	char	*result;

	if (shell->critical_er)
		return (NULL);
	result = ft_calloc(sizeof(char), ft_arraylen(head) + 1);
	if (!result)
		return (set_error(MALLOC_FAIL, shell), NULL);
	current = head;
	while (current->next)
	{
		token = (t_token *)current->content;
		if (current->content && ((t_token *)current->content)->content)
			result = ft_strjoinfree(result, (char *)token->content);
		if (!result)
			return (set_error(MALLOC_FAIL, shell), NULL);
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
	if (has_valid_var(node->content) && can_expand(node))
	{
		remove_quotes_from_string(shell, ptr_to_str);
		expand_string(shell, ptr_to_str);
	}
	else
		remove_quotes_from_string(shell, ptr_to_str);
}
