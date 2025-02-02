/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 14:37:02 by mkling            #+#    #+#             */
/*   Updated: 2025/02/02 15:22:35 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_error(void)
{
	ft_putstr_fd(SHELL_NAME, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
}

static char	*get_error_message(int err_code)
{
	if (err_code == MALLOC_FAIL)
		return ("Memory allocation failed");
	if (err_code == PIPE_ERROR)
		return ("Pipe failed");
	if (err_code == FORK_ERROR)
		return ("Fork failed");
	if (err_code == DUP_ERROR)
		return ("Redirection failed");
	if (err_code == NO_FILE)
		return ("No such file or directory");
	if (err_code == OPEN_ERROR)
		return ("Open failed");
	if (err_code == PERM_ERROR)
		return ("Permission denied");
	if (err_code == SYNTAX_ERROR)
		return ("Syntax error");
	if (err_code == IS_DIR)
		return ("Is a directory");
	if (err_code == IS_NOT_DIR)
		return ("Is not a directory");
	if (err_code == CANT_FIND_CMD)
		return ("Command not found");
	if (err_code == TOO_MANY_ARGS)
		return ("Too many arguments");
	return ("Error");
}

void	set_error(int err_code, t_shell *shell)
{
	print_error();
	ft_putstr_fd(get_error_message(err_code), STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	shell->critical_er = err_code;
}

void	set_cmd_error(int err_code, t_cmd *cmd, char *file_or_cmd)
{
	print_error();
	if (file_or_cmd != NULL)
	{
		ft_putstr_fd(file_or_cmd, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putstr_fd(get_error_message(err_code), STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	if (err_code == PERM_ERROR || err_code == PATH_ERROR
		|| err_code == IS_DIR || err_code == IS_NOT_DIR)
		cmd->exit_code = 126;
	else
		cmd->exit_code = err_code;
}

void	print_syntax_error(t_token *token)
{
	print_error();
	ft_putstr_fd("syntax error near unexpected token '", STDERR_FILENO);
	if (token->lexem == END)
		ft_putstr_fd("newline", STDERR_FILENO);
	else if (token->content)
		ft_putstr_fd(token->content, STDERR_FILENO);
	else
		ft_putchar_fd(token->letter, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
}
