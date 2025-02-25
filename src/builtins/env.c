/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgodoy <vgodoy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 18:10:02 by vgodoy            #+#    #+#             */
/*   Updated: 2025/02/25 22:48:32 by vgodoy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_env_key(char *env_key_and_value)
{
	char	*env_key;
	char	*equal_sign;
	int		equal_sign_index;

	equal_sign = ft_strchr(env_key_and_value, '=');
	if (equal_sign == NULL)
		return (ft_strdup(env_key_and_value));
	equal_sign_index = equal_sign - env_key_and_value;
	env_key = ft_calloc(equal_sign_index + 1, sizeof(char));
	ft_strlcat(env_key, env_key_and_value, equal_sign_index + 1);
	return (env_key);
}

char	*extract_env_value(char *env_key_and_value)
{
	char	*env_value;
	char	*equal_sign;
	char	*ptr_to_equal;
	int		value_len;

	ptr_to_equal = ft_strchr(env_key_and_value, '=');
	if (ptr_to_equal == NULL)
		return (ft_strdup(""));
	env_value = ft_calloc(ft_strlen(ptr_to_equal) - 1 + 1, sizeof(char));
	ft_strlcat(env_value, &ptr_to_equal[1], ft_strlen(ptr_to_equal) + 1);
	return (env_value);
}

static void	set_backup_environ(t_shell *shell)
{
	t_list	*pwd_env;
	t_list	*path_env;
	char	buffer[PATH_MAX];

	ft_bzero(buffer, PATH_MAX);
	pwd_env = find_env(shell->env_list, "PWD");
	if (!pwd_env || ft_strlen(pwd_env->content) < 5)
	{
		ft_strlcat(buffer, "PWD=", 5);
		if (getcwd(&buffer[4], PATH_MAX) == NULL)
			return ;
		replace_env(shell, buffer);
	}
}

void	extract_env_as_linked_list(t_shell *shell)
{
	int		index;

	index = 0;
	while (shell->env[index])
	{
		ft_lstadd_back(&shell->env_list,
			ft_lstnew(ft_strdup(shell->env[index])));
		index++;
	}
	set_backup_environ(shell);
}

int	env(t_shell *shell)
{
	t_list	*current;

	current = shell->env_list;
	while (current != NULL)
	{
		ft_putstr_fd((char *)current->content, STDOUT_FILENO);
		write(1, "\n", STDOUT_FILENO);
		current = current->next;
	}
	return (0);
}
