/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 18:11:08 by vgodoy            #+#    #+#             */
/*   Updated: 2025/02/25 18:44:59 by mkling           ###   ########.fr       */
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
