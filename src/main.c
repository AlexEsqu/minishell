/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 14:09:43 by mkling            #+#    #+#             */
/*   Updated: 2025/02/10 18:07:04 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
