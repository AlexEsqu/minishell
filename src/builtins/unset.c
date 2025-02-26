/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 18:10:25 by vgodoy            #+#    #+#             */
/*   Updated: 2025/02/26 13:37:42 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*find_env(t_shell *shell, t_list *env_list, char *env_name)
{
	char	*env_key;

	if (env_list == NULL || env_name == NULL)
		return (NULL);
	env_key = extract_env_key(shell, env_name);
	while (env_list)
	{
		if (ft_strncmp(env_key, (char *)env_list->content,
				ft_strlen(env_key)) == 0)
		{
			free(env_key);
			return (env_list);
		}
		env_list = env_list->next;
	}
	free(env_key);
	return (NULL);
}

int	unset(t_shell *shell, t_cmd *cmd)
{
	t_list	*current_arg;
	t_list	*env_to_be_unset;

	current_arg = cmd->arg_list->next;
	while (current_arg)
	{
		env_to_be_unset = find_env(shell, shell->env_list,
				(char *)current_arg->content);
		ft_lstpop(&shell->env_list, env_to_be_unset, free);
		current_arg = current_arg->next;
	}
	ft_free_tab(shell->env);
	shell->env = extract_list_as_array(shell, shell->env_list);
	if (!shell->env)
		return (set_error(MALLOC_FAIL, shell), MALLOC_FAIL);
	return (SUCCESS);
}
