/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 14:09:43 by mkling            #+#    #+#             */
/*   Updated: 2025/02/21 19:04:39 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;
	int		exit_return;

	shell = create_minishell(envp);
	if (argc > 2 && ft_strcmp(argv[1], "-c") == 0)
		parse_and_exec_cmd(shell, argv[2]);
	else if (!isatty(STDIN_FILENO))
		free_minishell(shell);
	else
		init_readline(shell);
	exit_return = shell->last_exit_code;
	free_minishell(shell);
	return (exit_return);
}
