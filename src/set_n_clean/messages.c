/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messages.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 15:44:51 by vgodoy            #+#    #+#             */
/*   Updated: 2025/02/24 13:31:30 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_error_message_1(int err_code)
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
	if (err_code == NON_NUM)
		return ("Numerical arguments required");
	if (err_code == TOO_MANY_ARGS)
		return ("Too many arguments");
	if (err_code == NO_HOME)
		return ("HOME not set");
	if (err_code == NO_PATH)
		return ("PATH not set");
	return ("Error");
}

char	*get_error_message(int err_code)
{
	if (err_code == IS_DIR)
		return ("Is a directory");
	if (err_code == IS_NOT_DIR)
		return ("Is not a directory");
	if (err_code == NO_CMD)
		return ("Command not found");
	if (err_code == TOO_MANY_ARGS)
		return ("Too many arguments");
	if (err_code == INVALID_VAR)
		return ("Not a valid identifier");
	if (err_code == INTERUPT)
		return (NULL);
	return (get_error_message_1(err_code));
}
