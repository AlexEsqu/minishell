/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 15:42:30 by mkling            #+#    #+#             */
/*   Updated: 2025/02/03 18:28:35 by mkling           ###   ########.fr       */
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

char	*generate_heredoc_filepath(t_shell *shell)
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

int	assemble_heredoc(t_shell *shell, t_cmd *cmd, char *end_of_file)
{
	char	*heredoc_filepath;
	char	*result;
	int		fd;

	heredoc_filepath = generate_heredoc_filepath(shell);
	if (!heredoc_filepath)
		return (set_cmd_error(MALLOC_FAIL, cmd, "Heredoc"), MALLOC_FAIL);
	fd = open(heredoc_filepath, O_RDWR | O_TRUNC | O_CREAT, 777);
	if (fd < 0)
		return (set_cmd_error(OPEN_ERROR, cmd, "Heredoc"), OPEN_ERROR);
	while (1)
	{
		write(STDIN_FILENO, "> ", 3);
		result = get_next_line(STDIN_FILENO);
		if (ft_strncmp(end_of_file, result, ft_strlen(end_of_file)) == 0)
		{
			free(result);
			break ;
		}
		write(fd, result, ft_strlen(result));
		free(result);
	}
	close(fd);
	cmd->fd_in = open(heredoc_filepath, O_RDONLY);
	if (cmd->fd_in < 0)
		return (set_cmd_error(OPEN_ERROR, cmd, "Heredoc"), OPEN_ERROR);
	ft_lstadd_back(&shell->heredoc, ft_lstnew(heredoc_filepath));
	return (SUCCESS);
}
