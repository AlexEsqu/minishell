/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 09:19:40 by skassimi          #+#    #+#             */
/*   Updated: 2025/01/30 16:32:06 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pwd(void)
{
	if (getenv("PWD") == NULL)
		return (-1);
	ft_putstr_fd(getenv("PWD"), STDOUT_FILENO);
	write(STDOUT_FILENO, "\n", 1);
	return (0);
}
