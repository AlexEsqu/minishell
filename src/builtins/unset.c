/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 09:55:19 by mkling            #+#    #+#             */
/*   Updated: 2025/01/30 09:58:23 by mkling           ###   ########.fr       */
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

int	unset(t_shell *shell, char **argv)
{
	int		i;
	t_list	*to_be_unset;

	i = 1;
	while (argv[i])
	{
		to_be_unset = find_env(shell->env_list, argv[i]);
		ft_lstpop(&shell->env_list, to_be_unset, free);
		i++;
	}
	shell->env = extract_list_as_array(shell, shell->env_list);
	return (0);
}
