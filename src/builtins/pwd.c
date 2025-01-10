/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 09:19:40 by skassimi          #+#    #+#             */
/*   Updated: 2024/12/19 14:20:02 by mkling           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

int	pwd(int fdout)
{
	if (getenv("PWD") == NULL || fdout < 0)
		return (-1);
	ft_putstr_fd(getenv("PWD"), fdout);
	write(fdout, "\n", 1);
	return (0);
}
