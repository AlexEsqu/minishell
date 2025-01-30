/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgodoy <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 10:36:09 by vgodoy            #+#    #+#             */
/*   Updated: 2025/01/30 10:36:35 by vgodoy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    handle_sigint(int sig)
{
        (void)sig;

        write(1, "\n", 1);
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
}

void    signals(void)
{
        signal(SIGINT, &handle_sigint);
        signal(SIGQUIT, SIG_IGN);
}
