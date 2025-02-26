/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 23:20:23 by vgodoy            #+#    #+#             */
/*   Updated: 2025/02/26 14:01:58 by alex             ###   ########.fr       */
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

int	replace_env(t_shell *shell, char *env_key_and_value)
{
	t_list	*to_be_replaced;
	char	*env_key;

	env_key = extract_env_key(shell, env_key_and_value);
	to_be_replaced = find_env(shell, shell->env_list, env_key);
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

int	export_valid_var(char *str)
{
	int	i;

	i = 1;
	if (!(ft_isalpha(str[0]) || str[0] == '_'))
		return (0);
	while (str[i])
	{
		if (str[i] == '=')
			break ;
		if (!(ft_isalpha(str[i]) || ft_isdigit(str[i])
				|| str[1] == '_'))
			return (0);
		i++;
	}
	return (1);
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
		if (!export_valid_var(cmd->argv[i]))
			return (set_cmd_error(INVALID_VAR, cmd, cmd->argv[i]), E_CMD_FAIL);
		ptr_to_equal_sign = ft_strchr(cmd->argv[i], '=');
		if (ptr_to_equal_sign && ptr_to_equal_sign[0] == '\0')
		{
			tmp = ft_strjoin(cmd->argv[i], "");
			if (!tmp)
				return (set_cmd_error(MALLOC_FAIL, cmd, NULL), MALLOC_FAIL);
			free(cmd->argv[i]);
			cmd->argv[i] = tmp;
		}
		if (replace_env(shell, cmd->argv[i]) != SUCCESS)
			return (set_cmd_error(MALLOC_FAIL, cmd, NULL), MALLOC_FAIL);
		i++;
	}
	return (0);
}
