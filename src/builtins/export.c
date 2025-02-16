/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 09:56:54 by mkling            #+#    #+#             */
/*   Updated: 2025/02/02 13:12:09 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_env_as_export(t_shell *shell)
{
	t_list	*current_env;

	current_env = shell->env_list;
	while (current_env != NULL)
	{
		ft_putstr_fd("export ", STDOUT_FILENO);
		ft_putstr_fd((char *)current_env->content, STDOUT_FILENO);
		write(STDOUT_FILENO, "\n", 1);
		current_env = current_env->next;
	}
}

char	*extract_env_key(char *env_key_and_value)
{
	char	*env_key;
	char	*equal_sign;
	int		equal_sign_pos;

	equal_sign = ft_strchr(env_key_and_value, '=');
	if (equal_sign == NULL)
		return (env_key_and_value);
	equal_sign_pos = equal_sign - env_key_and_value;
	env_key = ft_calloc(equal_sign_pos + 1, sizeof(char));
	ft_strlcat(env_key, env_key_and_value, equal_sign_pos + 1);
	return (env_key);
}

int	replace_env(t_shell *shell, char *env_key_and_value)
{
	t_list	*to_be_replaced;
	char	*env_key;

	env_key = extract_env_key(env_key_and_value);
	to_be_replaced = find_env(shell->env_list, env_key);
	if (!to_be_replaced)
	{
		to_be_replaced = ft_lstnew(ft_strdup(env_key_and_value));
		if (!to_be_replaced)
			return (MALLOC_FAIL);
		ft_lstadd_back(&shell->env_list, to_be_replaced);
	}
	else
	{
		free(to_be_replaced->content);
		to_be_replaced->content = env_key_and_value;
	}
	return (SUCCESS);
}

int	export(t_shell *shell, t_cmd *cmd)
{
	int	i;

	i = 1;
	if (cmd->argv[i] == NULL)
		return (print_env_as_export(shell), 0);
	while (cmd->argv[i])
	{
		if (replace_env(shell, cmd->argv[i]) != SUCCESS)
			return (set_cmd_error(MALLOC_FAIL, cmd, NULL), MALLOC_FAIL);
		i++;
	}
	return (0);
}
