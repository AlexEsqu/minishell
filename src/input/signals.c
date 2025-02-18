/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgodoy <vgodoy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 10:36:09 by vgodoy            #+#    #+#             */
/*   Updated: 2025/02/18 19:26:37 by vgodoy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int my_sig_nal = 0;

void	default_sig_nal(void)
{
	struct sigaction	sa_default;

	sa_default.sa_handler = SIG_DFL;
	sigemptyset(&sa_default.sa_mask);
	sa_default.sa_flags = 0;
	sigaction(SIGINT, &sa_default, NULL);
	sigaction(SIGQUIT, &sa_default, NULL);
}
void	deaf_mode(void)
{
	struct sigaction	sa_ignore;

	sa_ignore.sa_handler = SIG_IGN;
	sigemptyset(&sa_ignore.sa_mask);
	sa_ignore.sa_flags = 0;
	sigaction(SIGQUIT, &sa_ignore, NULL);
	sigaction(SIGINT, &sa_ignore, NULL);
}

void	heredoc_mode(int signal, siginfo_t *info, void *context)
{
	int	forkpid;

	(void)context;
	if (signal == SIGINT)
	{
		deaf_mode();
		my_sig_nal = CONTROL_C;
		// rl_on_new_line();
		// rl_replace_line("", 0);
		// rl_redisplay();

		// forkpid = fork();
		// if (forkpid == 0)
		// {
		// 	rl_on_new_line();
		// 	rl_replace_line("", 0);
		// 	rl_redisplay();
		// 	exit(52);
		// }
		// waitpid(forkpid, NULL, NULL);

		// write(STDIN_FILENO, "\x04", 1);

		//write(STDIN_FILENO, "\n", 1);

		// int dev_null = open("/dev/null", O_RDONLY);
		// dup2(dev_null, STDIN_FILENO);
		// close(dev_null);
	}
}

void	interactive_mode(int signal, siginfo_t *info, void *context)
{
	(void)context;
	if (signal == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		if (my_sig_nal != TYPING)
			rl_redisplay();
		my_sig_nal = CONTROL_C;
	}
}

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
		if (my_sig_nal != TYPING)//
			rl_redisplay();////////
		my_sig_nal = CONTROL_C;////
	}
}

int	signals(t_shell *shell, int mode)
{
	struct sigaction	sa_sig_nal;
	struct sigaction	sa_ignore;

	if (mode == NORMAL_MODE)
	{
		sa_sig_nal.sa_sigaction = normal_mode;
		sa_sig_nal.sa_flags = SA_SIGINFO;
		sigemptyset(&sa_sig_nal.sa_mask);
		sigaction(SIGINT, &sa_sig_nal, NULL);
		sigaction(SIGQUIT, &sa_sig_nal, NULL);
	}
	else if (mode == INTERACTIVE_MODE)
	{
		sa_sig_nal.sa_sigaction = interactive_mode;
		sa_sig_nal.sa_flags = SA_SIGINFO;
		sigemptyset(&sa_sig_nal.sa_mask);
		sigaction(SIGINT, &sa_sig_nal, NULL);

		sa_ignore.sa_handler = SIG_IGN;
		sigemptyset(&sa_ignore.sa_mask);
		sa_ignore.sa_flags = 0;
		sigaction(SIGQUIT, &sa_ignore, NULL);
	}
	else if (mode == HEREDOC_MODE)
	{
		sa_sig_nal.sa_sigaction = heredoc_mode;
		sa_sig_nal.sa_flags = SA_SIGINFO;
		sigemptyset(&sa_sig_nal.sa_mask);
		sigaction(SIGINT, &sa_sig_nal, NULL);

		sa_ignore.sa_handler = SIG_IGN;
		sigemptyset(&sa_ignore.sa_mask);
		sa_ignore.sa_flags = 0;
		sigaction(SIGQUIT, &sa_ignore, NULL);
	}
	return (1);
}
