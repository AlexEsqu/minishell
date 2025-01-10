/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 14:37:02 by mkling            #+#    #+#             */
/*   Updated: 2024/12/28 19:26:35 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_error(void)
{
	ft_putstr_fd(SHELL_NAME, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
}

void	set_error(int err_code, t_shell *shell, char *err_message)
{
	print_error();
	ft_putstr_fd(err_message, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	shell->critical_er = err_code;
}

void	set_cmd_error(int err_code, t_cmd *cmd, char *err_message)
{
	print_error();
	if (cmd->arg_list && (char *)cmd->arg_list->content)
	{
		ft_putstr_fd((char *)cmd->arg_list->content, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putstr_fd(err_message, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
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
