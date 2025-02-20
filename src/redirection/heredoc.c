/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgodoy <vgodoy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 15:40:31 by vgodoy            #+#    #+#             */
/*   Updated: 2025/02/20 18:30:32 by vgodoy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*Using a static counter
Give a new heredoc_path*/
static char	*generate_heredoc_filepath(t_shell *shell)
{
	static int	i;
	char		*itoa_i;
	char		*heredoc_path;

	heredoc_path = ft_strdup(HEREDOC_LOC);
	while (access(heredoc_path, F_OK) == 0)
	{
		free(heredoc_path);
		itoa_i = ft_itoa(i);
		if (!itoa_i)
			return (set_error(MALLOC_FAIL, shell), NULL);
		heredoc_path = ft_strjoin(HEREDOC_LOC, itoa_i);
		if (!heredoc_path)
			return (set_error(MALLOC_FAIL, shell), NULL);
		free(itoa_i);
		i++;
	}
	return (heredoc_path);
}

/*accumulate heredoc with readline, writing a line and the a /n
break the infinite loop:
- simple !line check for control+D
- custom signals for control+C
- strcmp for delimitator*/
static void	accumulate_heredoc_content(t_shell *shell, t_cmd *cmd, t_file *file)
{
	char	*line;

	while (1)
	{
		//my_sig_nal = IN_HEREDOC;
		signals(shell, HEREDOC_MODE);
		line = readline("here_doc$ ");
		signals(shell, NORMAL_MODE);
		if (control_d_pressed(line, file))
			break ;
		if (control_c_pressed(line, shell))
			break ;
		if (delim_summoned(line, file))
			break ;
		if (!file->is_quoted)
			expand_string(shell, &line);
		write(file->fd, line, ft_strlen(line));
		write(file->fd, "\n", 1);
		free(line);
	}
}

/*set the delim
check if it is quoted and removes quotes
generate a unique file path and opens it
accumulate heredoc content and closes file
clean up file path if control+C is used*/
void	assemble_heredoc(t_shell *shell, t_cmd *cmd, t_file *file)
{
	file->delim = file->path;
	if (ft_strchr(file->delim, '\'') || ft_strchr(file->delim, '\"'))
		file->is_quoted = true;
	remove_quotes_from_string(shell, &file->delim);
	file->path = generate_heredoc_filepath(shell);
	if (!file->path)
		return (set_cmd_error(MALLOC_FAIL, cmd, "Heredoc"));
	file->fd = open(file->path, O_RDWR | O_TRUNC | O_CREAT, 777);
	if (file->fd < 0)
		return (set_cmd_error(OPEN_ERROR, cmd, "Heredoc"));
	accumulate_heredoc_content(shell, cmd, file);
	close(file->fd);
	if (my_sig_nal == CONTROL_C)
		unlink(file->path);
}
