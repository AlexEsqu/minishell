/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readability.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 18:37:37 by mkling            #+#    #+#             */
/*   Updated: 2024/12/28 22:15:35 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_fork(t_shell *shell, int	*fork_pid)
{
	*fork_pid = fork();
	if (*fork_pid == -1)
		return (set_error(FORK_ERROR, shell, "Failed to fork"), FORK_ERROR);
	return (0);
}

int	create_pipe(t_shell *shell, int *pipe_fd)
{
	if (pipe(pipe_fd) != 0)
		return (set_error(PIPE_ERROR, shell, "Failed to pipe"), PIPE_ERROR);
	return (0);
}

void	close_pipe(int *pipe_fd)
{
	close(pipe_fd[READ]);
	close(pipe_fd[WRITE]);
}

void	reset_std(t_shell *shell, bool piped)
{
	if (piped)
		return ;
	dup2(shell->std_in, 0);
	dup2(shell->std_out, 1);
}

int	get_exit_code(int status)
{
	if (WIFSIGNALED(status))
		return(128 + WTERMSIG(status));
	return (WEXITSTATUS(status));
}

