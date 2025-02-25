/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgodoy <vgodoy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 15:39:37 by vgodoy            #+#    #+#             */
/*   Updated: 2025/02/25 23:18:45 by vgodoy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	normal_mode(int signal, siginfo_t *info, void *context)
{
	(void)context;
	if (signal == SIGQUIT)
	{
		printf("Quit (my core dumped)\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		g_my_sig = CONTROL_D;
	}
	else if (signal == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		if (g_my_sig != TYPING)
			rl_redisplay();
		g_my_sig = CONTROL_C;
	}
}

void	interactive_mode(int signal, siginfo_t *info, void *context)
{
	(void)context;
	if (signal == SIGINT)
	{
		g_my_sig = CONTROL_C;
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		if (g_my_sig != TYPING)
			rl_redisplay();
	}
}

void	heredoc_mode(int signal, siginfo_t *info, void *context)
{
	(void)context;
	if (signal == SIGINT)
	{
		g_my_sig = CONTROL_C;
		rl_done = 1;
		write(STDIN_FILENO, "\n", 2);
		printf(SHELL_PROMPT);
	}
}
