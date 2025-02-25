/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/02/25 18:52:40 by mkling           ###   ########.fr       */
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
			return (free(env_key), MALLOC_FAIL);
		ft_lstadd_back(&shell->env_list, to_be_replaced);
	}
	else
	{
		free(to_be_replaced->content);
		to_be_replaced->content = ft_strdup(env_key_and_value);
	}
	free(env_key);
	return (SUCCESS);
}

int	export(t_shell *shell, t_cmd *cmd)
{
	int		i;
	char	*ptr_to_equal_sign;
	char	*tmp;

	i = 1;
	if (cmd->argv[i] == NULL)
		return (print_env_as_export(shell), 0);
	while (cmd->argv[i])
	{
		if (cmd->argv[i][0] == '=' || cmd->argv[i][0] == '?'
			|| ft_strchr(cmd->argv[i], '$'))
			return (set_cmd_error(INVALID_VAR, cmd, cmd->argv[i]), E_CMD_FAIL);
		ptr_to_equal_sign = ft_strchr(cmd->argv[i], '=');
		if (ptr_to_equal_sign && ptr_to_equal_sign[0] == '\0')
		{
			tmp = ft_strjoin(cmd->argv[i], "");
			free(cmd->argv[i]);
			cmd->argv[i] = tmp;
		}
		if (replace_env(shell, cmd->argv[i]) != SUCCESS)
			return (set_cmd_error(MALLOC_FAIL, cmd, NULL), MALLOC_FAIL);
		i++;
	}
	return (0);
}
