/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgodoy <vgodoy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 10:36:09 by vgodoy            #+#    #+#             */
/*   Updated: 2025/02/14 16:53:19 by vgodoy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
//#include <signal.h>

int my_sig_nal = 0;

void	handle_signal(int signal, siginfo_t *info, void *context)
{
	(void)context;
	if (signal == SIGQUIT)
		return ;
	else if (signal == SIGINT)
	{
		printf("SIGINT received, pid=[%d]\n", info->si_pid);
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		if (my_sig_nal != TYPING)
			rl_redisplay();
		my_sig_nal = CONTROL_C;
	}
}

int	signals(t_shell *shell)
{
	struct sigaction	sa_server;

	sa_server.sa_sigaction = handle_signal;
	sa_server.sa_flags = SA_SIGINFO;
	sigemptyset(&sa_server.sa_mask);
	if (sigaction(SIGINT, &sa_server, NULL) == -1
		|| sigaction(SIGQUIT, &sa_server, NULL) == -1)
	{
		return (write(2, "Failed to set up signals\n", 26), 0);
	}
	return (1);
}
