/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell-salima.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 15:52:49 by mkling            #+#    #+#             */
/*   Updated: 2024/11/22 16:33:21 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_SALIMA_H
# define MINISHELL_SALIMA_H

typedef struct s_cmd
{
	char **argv;
	int argc;
	char *outfile;
	char *infile;
	s_cmd *next;
}	t_cmd;

typedef struct s_command_table
{
	t_cmd *first;
	int cmd_count;
}	t_command_table;

typedef struct s_maman
{
	t_command_table *cmd;
	char **line;
	char **env;
}	t_maman

#endif
