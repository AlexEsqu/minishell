/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 14:09:43 by mkling            #+#    #+#             */
/*   Updated: 2025/02/25 18:52:01 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*Initializes the shell environment
	Handles non-interactive execution (-c "command")
	If it's in interactive mode, starts readline
	Free memory before exiting*/
int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;
	int		exit_return;

	shell = create_minishell(envp);
	if (argc > 2 && ft_strcmp(argv[1], "-c") == 0)
		parse_and_exec_cmd(shell, argv[2]);
	else if (!isatty(STDIN_FILENO))
		return (free_minishell(shell), 0);
	else
		init_readline(shell);
	exit_return = shell->last_exit_code;
	free_minishell(shell);
	return (exit_return);
}
