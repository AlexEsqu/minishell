/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 09:20:16 by skassimi          #+#    #+#             */
/*   Updated: 2025/02/14 01:19:16 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	string_is_only_digit(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

/* Exit with first argument as exit code
Bash documentation points out arg higher than 255 is undefined
Here supports up to int max, then throws syntax error */
int	exit_shell(t_shell *shell, t_cmd *cmd)
{
	int	exit_code;

	if (cmd->argc > 2)
		return (set_cmd_error(TOO_MANY_ARGS, cmd, cmd->argv[0]), TOO_MANY_ARGS);
	if (cmd->argc > 1)
	{
		if (!string_is_only_digit(cmd->argv[1])
			|| is_too_long_for_int(cmd->argv[1]))
		{
			print_error();
			ft_putstr_fd("exit: ", STDERR_FILENO);
			ft_putstr_fd("Requires numerical arguments\n", STDERR_FILENO);
			exit_code = 2;
		}
		else
		{
			exit_code = ft_atoi(cmd->argv[1]);
			while (exit_code > 255)
				exit_code -= 255;
		}
	}
	else
		exit_code = shell->last_exit_code;
	free_minishell(shell);
	exit(exit_code);
}
