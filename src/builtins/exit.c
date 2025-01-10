/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 09:20:16 by skassimi          #+#    #+#             */
/*   Updated: 2024/12/26 15:13:29 by alex             ###   ########.fr       */
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
int	exit_shell(t_shell *shell, char **argv)
{
	int	exit_code;
	int	i;

	i = 1;
	if (argv[i])
	{
		if (argv[i + 1])
		{
			print_error();
			ft_putstr_fd("exit: Too many arguments\n", STDERR_FILENO);
		}
		if (!string_is_only_digit(argv[i]) || is_too_long_for_int(argv[i]))
		{
			print_error();
			ft_putstr_fd("exit: Requires numerical arguments\n", STDERR_FILENO);
		}
		exit_code = ft_atoi(argv[i]);
		while (exit_code > 255)
			exit_code -= 255;
	}
	else
		exit_code = shell->last_exit_code;
	free_minishell(shell);
	exit(exit_code);
}
