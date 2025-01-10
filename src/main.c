/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 14:09:43 by mkling            #+#    #+#             */
/*   Updated: 2024/12/19 14:22:10 by mkling           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

// int	main(int argc, char **argv, char **envp)
// {
// 	t_shell	*shell;
// 	char	*false_argv[2] = {"export", NULL};

// 	shell = create_minishell(envp);
// 	if (argc > 1)
// 	{
// 		export(shell, &argv[1], STDOUT_FILENO);
// 		fprintf(stderr, "export done\n\n");
// 		export(shell, false_argv, STDOUT_FILENO);
// 		unset(shell, &argv[1]);
// 		fprintf(stderr, "unset done\n\n");
// 		export(shell, false_argv, STDOUT_FILENO);
// 	}
// 	return (0);
// }

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;

	shell = create_minishell(envp);
	if (argc > 2 && ft_strcmp(argv[1], "-c") == 0)
		parse_and_exec_cmd(shell, argv[2]);
	else
		init_readline(shell);
	free_minishell(shell);
	return (0);
}
