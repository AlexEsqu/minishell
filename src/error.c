/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 11:03:13 by mkling            #+#    #+#             */
/*   Updated: 2024/11/21 16:44:43 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	exit_if(bool condition, char *err_message, int err_code, void *data)
{
	if (condition == true)
	{
		free(data);
		if (err_message != NULL)
			ft_putstr_fd(err_message, 2);
		exit(err_code);
	}
}
