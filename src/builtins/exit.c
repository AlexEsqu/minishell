/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 09:20:16 by skassimi          #+#    #+#             */
/*   Updated: 2025/02/18 10:36:38 by mkling           ###   ########.fr       */
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

void	print_exit_error(int err_code)
{
	print_error();
	ft_putstr_fd("exit: ", STDERR_FILENO);
	if (err_code == TOO_MANY_ARGS)
		ft_putstr_fd("Too many arguments\n", STDERR_FILENO);
	else
		ft_putstr_fd("Requires numerical arguments\n", STDERR_FILENO);
}

/* Exit with first argument as exit code
Bash documentation points out arg higher than 255 is undefined
Here supports up to int max, then throws syntax error */
int	exit_shell(t_shell *shell, t_cmd *cmd)
{
	int	exit_code;

	if (cmd->argc > 2 && string_is_only_digit(cmd->argv[1]))
		return (set_cmd_error(TOO_MANY_ARGS, cmd, cmd->argv[0]), TOO_MANY_ARGS);
	else if (cmd->argc > 1)
	{
		if (!string_is_only_digit(cmd->argv[1])
			|| is_too_long_for_int(cmd->argv[1]))
		{
			print_exit_error(NON_NUM);
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
