/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgodoy <vgodoy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 18:11:08 by vgodoy            #+#    #+#             */
/*   Updated: 2025/02/20 20:10:07 by vgodoy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* This function is the core of algo-shell program,
handling both interactive and non-interactive command execution.
    Initializes the shell environment.
    Handles non-interactive execution (-c "command").
    Determines if it's running in interactive mode and starts readline.
    Frees memory before exiting.*/
int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;

	shell = create_minishell(envp);
	if (argc > 2 && ft_strcmp(argv[1], "-c") == 0)
		parse_and_exec_cmd(shell, argv[2]);
	else if (!isatty(STDIN_FILENO))
		free_minishell(shell);
	else
		init_readline(shell);
	free_minishell(shell);
	return (0);
}
