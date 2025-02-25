/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interruptions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgodoy <vgodoy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 15:40:20 by vgodoy            #+#    #+#             */
/*   Updated: 2025/02/25 23:17:15 by vgodoy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	control_d_pressed(char *line, t_file *file)
{
	if (!line)
	{
		printf("pas cool, t'aurais pu mettre le delim");
		printf(" [%s] que t'as choisi\n", file->delim);
		return (1);
	}
	return (0);
}

int	control_c_pressed(char *line, t_shell *shell)
{
	if (g_my_sig == CONTROL_C)
	{
		free(line);
		shell->last_exit_code = E_SIG_INT;
		return (1);
	}
	return (0);
}

int	delim_summoned(char *line, t_file *file)
{
	int	line_len;
	int	delim_len;

	line_len = ft_strlen(line);
	delim_len = ft_strlen(file->delim);
	if (line_len > 0 && delim_len == line_len
		&& ft_strncmp(file->delim, line, line_len) == 0)
	{
		free(line);
		return (1);
	}
	return (0);
}
