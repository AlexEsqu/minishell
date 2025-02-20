/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgodoy <vgodoy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 17:22:48 by vgodoy            #+#    #+#             */
/*   Updated: 2025/02/20 20:11:43 by vgodoy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_error(void)
{
	ft_putstr_fd(SHELL_NAME, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
}

void	set_error(int err_code, t_shell *shell)
{
	char	*error_message;

	error_message = get_error_message(err_code);
	print_error();
	ft_putstr_fd(error_message, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	if (shell->tree_root)
		free_tree(&shell->tree_root);
	shell->critical_er = err_code;
	if (shell->critical_er == SYNTAX_ERROR)
		shell->last_exit_code = E_SYNTAX;
	else
		shell->last_exit_code = E_CMD_FAIL;
}

void	set_cmd_error(int err_code, t_cmd *cmd, char *file_or_cmd)
{
	char	*error_message;

	error_message = get_error_message(err_code);
	if (!error_message)
		return ;
	print_error();
	if (file_or_cmd != NULL)
	{
		ft_putstr_fd(file_or_cmd, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putstr_fd(get_error_message(err_code), STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	if (err_code == PERM_ERROR || err_code == IS_DIR
		|| err_code == IS_NOT_DIR)
		cmd->exit_code = E_NO_PERM;
	else if (err_code == PATH_ERROR || err_code == NO_CMD)
		cmd->exit_code = E_NO_CMD;
	else
		cmd->exit_code = E_SYNTAX;
}

void	print_syntax_error(t_shell *shell, t_token *token)
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
	shell->critical_er = SYNTAX_ERROR;
	shell->last_exit_code = E_SYNTAX;
}
