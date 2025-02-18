/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgodoy <vgodoy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 10:36:09 by vgodoy            #+#    #+#             */
/*   Updated: 2025/02/18 10:51:46 by vgodoy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
//#include <signal.h>

int my_sig_nal = 0;

// struct sigaction sa_ignore;
// sa_ignore.sa_handler = SIG_IGN;  // Ignorer le signal
// sigemptyset(&sa_ignore.sa_mask);
// sa_ignore.sa_flags = 0;

// sigaction(SIGINT, &sa_ignore, NULL);


// struct sigaction sa_default;
// sa_default.sa_handler = SIG_DFL; // Rétablir le comportement par défaut
// sigemptyset(&sa_default.sa_mask);
// sa_default.sa_flags = 0;

// sigaction(SIGINT, &sa_default, NULL);
// sigaction(SIGQUIT, &sa_default, NULL);


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
