/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 15:42:30 by mkling            #+#    #+#             */
/*   Updated: 2025/01/12 00:58:41 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* PIPE METHOD */

void	handle_heredoc(int sig)
{
	printf("\n");
	rl_replace_line(NULL, 0);
	(void) sig;
}

int	assemble_heredoc(t_shell *shell, t_cmd *cmd, t_list *file_node)
{
	int		fd[2];
	char	*line;
	t_file	*file;

	file = (t_file *)file_node->content;
	if (create_pipe(shell, fd) != 0)
		return (set_cmd_error(PIPE_ERROR, cmd, NULL), PIPE_ERROR);
	while (true)
	{
		// signal(SIGINT, heredoc_handler);
		line = readline("> ");
		// signal(SIGINT, signal_handler);
		if (!ft_strcmp(line, (file->delim)) || !line)
		{
			close(fd[1]);
			file->fd = fd[0];
			free(line);
			return (fd[0]);
		}
		ft_putstr_fd(line, fd[1]);
		ft_putstr_fd("\n", fd[1]);
		free(line);
	}
}

/* FILE METHOD */
// char	*increment_char(t_shell *shell, char *string)
// {
// 	size_t	len;
// 	char	letter;

// 	if (!string)
// 		return (NULL);
// 	letter = 'a';
// 	len = ft_strlen(string);
// 	if (len > ft_strlen(HEREDOC_LOC)
// 		|| string[len - 1] != 'z' || string[len - 1] != 'Z')
// 	{
// 		string[len - 1] = string[len - 1] + 1;
// 		return (string);
// 	}
// 	else if (len < 256)
// 		return (ft_strjoinfree(string, &letter));
// 	return (set_error(TOO_MANY_HEREDOC, shell,
// 			"reached end of posible random name"), NULL);
// }

// char	*generate_heredoc_filepath(t_shell *shell)
// {
// 	char	*heredoc_path;

// 	heredoc_path = ft_strdup(HEREDOC_LOC);
// 	while (1)
// 	{
// 		if (access(heredoc_path, F_OK) != 0)
// 			break ;
// 		heredoc_path = increment_char(shell, heredoc_path);
// 	}
// 	return (heredoc_path);
// }

// void	assemble_heredoc(t_shell *shell, t_cmd *cmd, t_list *file_node)
// {
// 	t_file	*file;
// 	char	*result;
// 	char	*heredoc_index;

// 	file = (t_file *)file_node->content;
// 	if (file->mode != HEREDOC)
// 		return ;
// 	open_file(file, cmd, WRITE);
// 	if (file->fd < 0)
// 		return (set_error(READ_ERROR, shell, "Unable to create heredoc"));
// 	heredoc_index = ft_strjoinfree(ft_itoa(cmd->cmd_index + 1), " heredoc> ");
// 	while (1)
// 	{
// 		write(STDIN_FILENO, heredoc_index, ft_strlen(heredoc_index));
// 		result = get_next_line(STDIN_FILENO);
// 		if (ft_strncmp(file->delim, result, ft_strlen(file->delim)) == 0)
// 			break ;
// 		write(file->fd, result, ft_strlen(result));
// 	}
// 	close(file->fd);
// }

// void	destroy_heredoc(t_shell *shell, t_list *file_node)
// {
// 	t_file	*file;

// 	if (shell->critical_er)
// 		return ;
// 	file = (t_file *)(file_node)->content;
// 	if (file->mode != HEREDOC)
// 		return ;
// 	unlink(file->path);
// }
