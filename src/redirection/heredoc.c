/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgodoy <vgodoy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 15:42:30 by mkling            #+#    #+#             */
/*   Updated: 2025/02/18 19:57:34 by vgodoy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* PIPE METHOD */

// void	handle_heredoc(int sig)
// {
// 	printf("\n");
// 	rl_replace_line(NULL, 0);
// 	(void) sig;
// }

// int	assemble_heredoc(t_shell *shell, t_cmd *cmd, char *end_of_file)
// {
// 	int		fd[2];
// 	char	*line;

// 	if (create_pipe(shell, fd) != 0)
// 		return (set_cmd_error(PIPE_ERROR, cmd, NULL), PIPE_ERROR);
// 	while (true)
// 	{
// 		// signal(SIGINT, heredoc_handler);
// 		line = readline("> ");
// 		// signal(SIGINT, signal_handler);
// 		if (!ft_strcmp(line, end_of_file) || !line)
// 		{
// 			close(fd[1]);
// 			free(line);
// 			return (fd[0]);
// 		}
// 		ft_putstr_fd(line, fd[1]);
// 		ft_putstr_fd("\n", fd[1]);
// 		free(line);
// 	}
// }

/* FILE METHOD */

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

static void	accumulate_heredoc_content(t_shell *shell, t_cmd *cmd, t_file *file)
{
	char	*line;
	char	*tmp;

	while (1)
	{
		my_sig_nal = IN_HEREDOC;
		signals(shell, HEREDOC_MODE);
		line = readline("here_doc$ ");
		signals(shell, NORMAL_MODE);
		if (my_sig_nal == CONTROL_C)
		{
			free(line);
			shell->last_exit_code = E_SIG_INT;
			break ;
		}
		else if (!line)
		{
			printf("pas cool, t'aurais pu mettre le delim [%s] que t'as choisi\n", file->delim);
			free(line);
			break ;
		}
		else if (ft_strncmp(file->delim, line, ft_strlen(line)) == 0)
		{
			free(line);
			break ;
		}
		if (!file->is_quoted)
			expand_string(shell, &line);
		write(file->fd, line, ft_strlen(line));
		write(file->fd, "\n", 1);
		free(line);
	}
}

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
