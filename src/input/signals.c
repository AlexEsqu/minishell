/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgodoy <vgodoy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 10:36:09 by vgodoy            #+#    #+#             */
/*   Updated: 2025/02/25 23:26:26 by vgodoy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_my_sig = 0;

void	signal_normal(void)
{
	struct sigaction	sa_sig_nal;

	sa_sig_nal.sa_sigaction = normal_mode;
	sa_sig_nal.sa_flags = SA_SIGINFO;
	sigemptyset(&sa_sig_nal.sa_mask);
	sigaction(SIGINT, &sa_sig_nal, NULL);
	sigaction(SIGQUIT, &sa_sig_nal, NULL);
}

void	signal_interactive(void)
{
	struct sigaction	sa_sig_nal;
	struct sigaction	sa_ignore;

	sa_sig_nal.sa_sigaction = interactive_mode;
	sa_sig_nal.sa_flags = SA_SIGINFO;
	sigemptyset(&sa_sig_nal.sa_mask);
	sigaction(SIGINT, &sa_sig_nal, NULL);
	sa_ignore.sa_handler = SIG_IGN;
	sa_ignore.sa_flags = 0;
	sigemptyset(&sa_ignore.sa_mask);
	sigaction(SIGQUIT, &sa_ignore, NULL);
}

void	signal_heredoc(void)
{
	struct sigaction	sa_sig_nal;
	struct sigaction	sa_ignore;

	g_my_sig = IN_HEREDOC;
	sa_sig_nal.sa_sigaction = heredoc_mode;
	sa_sig_nal.sa_flags = SA_SIGINFO;
	sigemptyset(&sa_sig_nal.sa_mask);
	sigaction(SIGINT, &sa_sig_nal, NULL);
	sa_ignore.sa_handler = SIG_IGN;
	sa_ignore.sa_flags = 0;
	sigemptyset(&sa_ignore.sa_mask);
	sigaction(SIGQUIT, &sa_ignore, NULL);
}

void	signals(t_shell *shell, int mode)
{
	if (mode == NORMAL_MODE)
		signal_normal();
	else if (mode == INTERACTIVE_MODE)
		signal_interactive();
	else if (mode == HEREDOC_MODE)
		signal_heredoc();
}
