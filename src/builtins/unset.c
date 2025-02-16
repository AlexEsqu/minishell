/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 09:55:19 by mkling            #+#    #+#             */
/*   Updated: 2025/02/02 12:54:24 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*find_env(t_list *env_list, char *env_name)
{
	if (env_list == NULL || env_name == NULL)
		return (NULL);
	while (env_list)
	{
		if (ft_strncmp(env_name, (char *)env_list->content,
				ft_strlen(env_name)) == 0)
			return (env_list);
		env_list = env_list->next;
	}
	return (NULL);
}

int	unset(t_shell *shell, t_cmd *cmd)
{
	t_list	*current_arg;
	t_list	*env_to_be_unset;

	current_arg = cmd->arg_list->next;
	while (current_arg)
	{
		env_to_be_unset = find_env(shell->env_list, (char *)current_arg->content);
		ft_lstpop(&shell->env_list, env_to_be_unset, free);
		current_arg = current_arg->next;
	}
	shell->env = extract_list_as_array(shell, shell->env_list);
	return (SUCCESS);
}
