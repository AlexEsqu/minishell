/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgodoy <vgodoy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 23:20:39 by vgodoy            #+#    #+#             */
/*   Updated: 2025/02/25 23:20:41 by vgodoy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	string_is_only_digit_or_sign(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]) && str[i] != '+' && str[i] != '-')
			return (0);
		i++;
	}
	return (1);
}

void	print_exit_error(int err_code)
{
	print_error();
	ft_putstr_fd("exit: ", STDERR_FILENO);
	if (err_code == NON_NUM)
		ft_putstr_fd("Requires numerical arguments\n", STDERR_FILENO);
	if (err_code == TOO_MANY_ARGS)
		ft_putstr_fd("Too many arguments\n", STDERR_FILENO);
}

/* Exit with first argument as exit code
Bash documentation points out arg higher than 255 is undefined
Here supports up to int max, then throws syntax error */
int	exit_shell(t_shell *shell, t_cmd *cmd)
{
	long long	exit_code;

	if (cmd->argc == 1)
		exit_code = shell->last_exit_code;
	else if (!string_is_only_digit_or_sign(cmd->argv[1])
		|| is_too_long_for_long_long(cmd->argv[1]))
	{
		set_cmd_error(NON_NUM, cmd, NULL);
		exit_code = E_SYNTAX;
	}
	else if (cmd->argc > 2)
		return (set_cmd_error(TOO_MANY_ARGS, cmd, cmd->argv[0]),
			E_CMD_FAIL);
	else
	{
		exit_code = ft_atol(cmd->argv[1]);
		if (exit_code > 255)
			exit_code = exit_code % 256;
	}
	free_minishell(shell);
	exit(exit_code);
	return (set_cmd_error(E_CMD_FAIL, cmd, NULL), E_CMD_FAIL);
}
