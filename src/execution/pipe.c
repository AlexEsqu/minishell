/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 00:30:00 by alex              #+#    #+#             */
/*   Updated: 2025/02/21 19:36:07 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_fork(t_shell *shell, int	*fork_pid)
{
	*fork_pid = fork();
	if (*fork_pid == -1)
		return (set_error(FORK_ERROR, shell), FORK_ERROR);
	return (0);
}

static int	create_pipe_and_fork(t_shell *shell, int *pipe_fd, int *fork_pid)
{
	if (pipe(pipe_fd) != 0)
		return (set_error(PIPE_ERROR, shell), PIPE_ERROR);
	*fork_pid = fork();
	if (*fork_pid == -1)
		return (set_error(FORK_ERROR, shell), FORK_ERROR);
	return (0);
}

static void	clean_fork_exit(t_shell *shell, int err_code)
{
	if (err_code != E_CMD_FAIL)
		set_error(err_code, shell);
	free_minishell(shell);
	exit(E_CMD_FAIL);
}

static void	exec_pipe_forks(t_shell *s, t_tree *tree, int *forkpid, int *pipefd)
{
	if (forkpid[0] == 0)
	{
		close(pipefd[READ]);
		if (dup2(pipefd[WRITE], STDOUT_FILENO) == -1)
			clean_fork_exit(s, DUP_ERROR);
		close(pipefd[WRITE]);
		exec_tree(s, tree->left, true);
		clean_fork_exit(s, E_CMD_FAIL);
	}
	if (forkpid[1] == 0)
	{
		close(pipefd[WRITE]);
		if (dup2(pipefd[READ], STDIN_FILENO) == -1)
			clean_fork_exit(s, DUP_ERROR);
		close(pipefd[READ]);
		exec_tree(s, tree->right, true);
		clean_fork_exit(s, E_CMD_FAIL);
	}
}

int	exec_pipe_monitor(t_shell *shell, t_tree *tree)
{
	int	fork_pid[2];
	int	pipe_fd[2];
	int	exit_code;

	if (create_pipe_and_fork(shell, pipe_fd, &fork_pid[0]) != SUCCESS)
		return (shell->critical_er);
	if (fork_pid[0] == 0)
		exec_pipe_forks(shell, tree, fork_pid, pipe_fd);
	if (create_fork(shell, &fork_pid[1]) != SUCCESS)
		return (FORK_ERROR);
	if (fork_pid[1] == 0)
		exec_pipe_forks(shell, tree, fork_pid, pipe_fd);
	close(pipe_fd[READ]);
	close(pipe_fd[WRITE]);
	waitpid(fork_pid[0], &exit_code, 0);
	waitpid(fork_pid[1], &exit_code, 0);
	return (WEXITSTATUS(exit_code));
}
