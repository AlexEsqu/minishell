/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 18:10:02 by vgodoy            #+#    #+#             */
/*   Updated: 2025/02/26 14:09:21 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_env_key(t_shell *shell, char *env_key_and_value)
{
	char	*env_key;
	char	*equal_sign;
	int		equal_sign_index;

	equal_sign = ft_strchr(env_key_and_value, '=');
	if (equal_sign == NULL)
	{
		env_key = ft_strdup(env_key_and_value);
		if (!env_key)
			return (set_error(MALLOC_FAIL, shell), NULL);
	}
	else
	{
		equal_sign_index = equal_sign - env_key_and_value;
		env_key = ft_calloc(equal_sign_index + 1, sizeof(char));
		if (!env_key)
			return (set_error(MALLOC_FAIL, shell), NULL);
		ft_strlcat(env_key, env_key_and_value, equal_sign_index + 1);
	}
	return (env_key);
}

char	*extract_env_value(t_shell *shell, char *env_key_and_value)
{
	char	*env_value;
	char	*equal_sign;
	char	*ptr_to_equal;
	int		value_len;

	ptr_to_equal = ft_strchr(env_key_and_value, '=');
	if (ptr_to_equal == NULL)
	{
		env_value = ft_strdup("");
		if (!env_value)
			return (set_error(MALLOC_FAIL, shell), NULL);
	}
	else
	{
		env_value = ft_calloc(ft_strlen(ptr_to_equal) - 1 + 1, sizeof(char));
		if (!env_value)
			return (set_error(MALLOC_FAIL, shell), NULL);
		ft_strlcat(env_value, &ptr_to_equal[1], ft_strlen(ptr_to_equal) + 1);
	}
	return (env_value);
}

static void	set_backup_environ(t_shell *shell)
{
	t_list	*pwd_env;
	t_list	*path_env;
	char	buffer[PATH_MAX];

	ft_bzero(buffer, PATH_MAX);
	pwd_env = find_env(shell, shell->env_list, "PWD");
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
	char	*env_content;

	index = 0;
	while (shell->env[index])
	{
		env_content = ft_strdup(shell->env[index]);
		if (!env_content)
			return (set_error(MALLOC_FAIL, shell));
		ft_lstadd_back(&shell->env_list, ft_lstnew(env_content));
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
