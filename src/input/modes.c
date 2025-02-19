/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgodoy <vgodoy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 15:39:37 by vgodoy            #+#    #+#             */
/*   Updated: 2025/02/19 15:39:38 by vgodoy           ###   ########.fr       */
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
		my_sig_nal = CONTROL_D;
	}
	else if (signal == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		if (my_sig_nal != TYPING)
			rl_redisplay();
		my_sig_nal = CONTROL_C;
	}
}

void	interactive_mode(int signal, siginfo_t *info, void *context)
{
	(void)context;
	if (signal == SIGINT)
	{
		my_sig_nal = CONTROL_C;
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		if (my_sig_nal != TYPING)
			rl_redisplay();
	}
}

void	heredoc_mode(int signal, siginfo_t *info, void *context)
{

	(void)context;
	if (signal == SIGINT)
	{
		my_sig_nal = CONTROL_C;
		rl_done = 1;
		write(STDIN_FILENO, "\n", 2);
		printf(SHELL_PROMPT);
	}
}
/*
void	deaf_mode(void)
{
	struct sigaction	sa_ignore;

	sa_ignore.sa_handler = SIG_IGN;
	sigemptyset(&sa_ignore.sa_mask);
	sa_ignore.sa_flags = 0;
	sigaction(SIGQUIT, &sa_ignore, NULL);
	sigaction(SIGINT, &sa_ignore, NULL);
}

void	default_mode(void)
{
	struct sigaction	sa_default;

	sa_default.sa_handler = SIG_DFL;
	sigemptyset(&sa_default.sa_mask);
	sa_default.sa_flags = 0;
	sigaction(SIGINT, &sa_default, NULL);
	sigaction(SIGQUIT, &sa_default, NULL);
}
*/

