/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   escape.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 11:55:48 by alex              #+#    #+#             */
/*   Updated: 2025/02/24 08:29:38 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	id_escaped_char(t_shell *shell, t_list *node)
{
	t_token	*backslash;
	t_token	*next_token;

	backslash = ((t_token *)node->content);
	if (backslash->letter != '\\')
		return ;
	next_token = ((t_token *)node->next->content);
	backslash->lexem = WORD;
	free(backslash->content);
	if (next_token->lexem != END && next_token->lexem != BLANK)
	{
		backslash->content = ft_strdup(next_token->content);
		ft_lstpop(&node, node->next, free_token);
	}
	else
		backslash->content = ft_strdup("");
}

