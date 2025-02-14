/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 00:30:00 by alex              #+#    #+#             */
/*   Updated: 2025/02/14 10:50:43 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	create_pipe_and_fork(t_shell *shell, int *pipe_fd, int *fork_pid)
{
	if (pipe(pipe_fd) != 0)
		return (set_error(PIPE_ERROR, shell), PIPE_ERROR);
	*fork_pid = fork();
	if (*fork_pid == -1)
		return (set_error(FORK_ERROR, shell), FORK_ERROR);
	return (0);
}

static void	clean_fork_exit(t_shell *shell)
{
	free_minishell(shell);
	exit(E_CMD_FAIL);
}

static int	exec_pipe_forks(t_shell *shell, t_tree *tree)
{
	int	pipe_fd[2];
	int	second_fork_pid;
	int	exit_code;

	if (create_pipe_and_fork(shell, pipe_fd, &second_fork_pid) != 0)
		return (shell->critical_er);
	if (second_fork_pid == 0)
	{
		close(pipe_fd[READ]);
		if (dup2(pipe_fd[WRITE], STDOUT_FILENO) == -1)
			return (set_error(DUP_ERROR, shell), DUP_ERROR);
		close(pipe_fd[WRITE]);
		exit_code = exec_tree(shell, tree->left, true);
		clean_fork_exit(shell);
	}
	else
	{
		close(pipe_fd[WRITE]);
		if (dup2(pipe_fd[READ], STDIN_FILENO) == -1)
			return (set_error(DUP_ERROR, shell), DUP_ERROR);
		close(pipe_fd[READ]);
		exit_code = exec_tree(shell, tree->right, true);
		clean_fork_exit(shell);
	}
	return (WEXITSTATUS(exit_code));
}

int	exec_pipe_monitor(t_shell *shell, t_tree *tree)
{
	int	fork_pid[2];
	int	exit_code;

	if (create_fork(shell, &fork_pid[0]))
		return (set_error(FORK_ERROR, shell), FORK_ERROR);
	fork_pid[1] = -1;
	if (fork_pid[0] == 0)
		exec_pipe_forks(shell, tree);
	else
	{
		waitpid(fork_pid[1], &exit_code, 0);
		waitpid(fork_pid[0], &exit_code, 0);
		return (WEXITSTATUS(exit_code));
	}
	return (set_error(FORK_ERROR, shell), FORK_ERROR);
}
