/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgodoy <vgodoy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 10:36:09 by vgodoy            #+#    #+#             */
/*   Updated: 2025/02/27 11:17:12 by vgodoy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_my_sig = 0;

int	sig_nal_test(void)
{
	struct sigaction	sa_default;

	sa_default.sa_handler = SIG_DFL;
	sigemptyset(&sa_default.sa_mask);
	sa_default.sa_flags = 0;
	if (sigaction(SIGINT, &sa_default, NULL) == -1)
		return (0);
	if (sigaction(SIGQUIT, &sa_default, NULL) == -1)
		return (0);
	return (1);
}

int	signal_normal(void)
{
	struct sigaction	sa_sig_nal;

	sa_sig_nal.sa_sigaction = normal_mode;
	sa_sig_nal.sa_flags = SA_SIGINFO;
	sigemptyset(&sa_sig_nal.sa_mask);
	if (sigaction(SIGINT, &sa_sig_nal, NULL) == -1)
		return (0);
	if (sigaction(SIGQUIT, &sa_sig_nal, NULL) == -1)
		return (0);
	return (1);
}

int	signal_interactive(void)
{
	struct sigaction	sa_sig_nal;
	struct sigaction	sa_ignore;

	sa_sig_nal.sa_sigaction = interactive_mode;
	sa_sig_nal.sa_flags = SA_SIGINFO;
	sigemptyset(&sa_sig_nal.sa_mask);
	if (sigaction(SIGINT, &sa_sig_nal, NULL) == -1)
		return (0);
	sa_ignore.sa_handler = SIG_IGN;
	sa_ignore.sa_flags = 0;
	sigemptyset(&sa_ignore.sa_mask);
	if (sigaction(SIGQUIT, &sa_ignore, NULL) == -1)
		return (0);
	return (1);
}

int	signal_heredoc(void)
{
	struct sigaction	sa_sig_nal;
	struct sigaction	sa_ignore;

	g_my_sig = IN_HEREDOC;
	sa_sig_nal.sa_sigaction = heredoc_mode;
	sa_sig_nal.sa_flags = SA_SIGINFO;
	sigemptyset(&sa_sig_nal.sa_mask);
	if (sigaction(SIGINT, &sa_sig_nal, NULL) == -1)
		return (0);
	sa_ignore.sa_handler = SIG_IGN;
	sa_ignore.sa_flags = 0;
	sigemptyset(&sa_ignore.sa_mask);
	if (sigaction(SIGQUIT, &sa_ignore, NULL) == -1)
		return (0);
	return (1);
}

int	signals(t_shell *shell, int mode)
{
	if (mode == TEST_MODE)
	{
		if (!sig_nal_test())
			return (set_error(SIGNAL_ERROR, shell), 0);
	}
	if (mode == NORMAL_MODE)
	{
		if (!signal_normal())
			return (set_error(SIGNAL_ERROR, shell), 0);
	}
	else if (mode == INTERACTIVE_MODE)
	{
		if (!signal_interactive())
			return (set_error(SIGNAL_ERROR, shell), 0);
	}
	else if (mode == HEREDOC_MODE)
	{
		if (!signal_heredoc())
			return (set_error(SIGNAL_ERROR, shell), 0);
	}
	return (1);
}
