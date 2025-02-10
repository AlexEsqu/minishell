/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 00:22:21 by alex              #+#    #+#             */
/*   Updated: 2025/02/10 22:40:44 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_token(void *to_be_del)
{
	t_token	*token;

	token = (t_token *)to_be_del;
	if (token == NULL)
		return ;
	if (token->content != NULL)
		free(((t_token *)token)->content);
	free(token);
	to_be_del = NULL;
}

void	free_cmd(void *to_be_del)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)to_be_del;
	if (!cmd)
		return ;
	if (cmd->argv)
		free(cmd->argv);
	if (cmd->cmd_path)
		free(cmd->cmd_path);
	if (cmd->arg_list)
	{
		ft_lstclear(&cmd->arg_list, free);
		cmd->arg_list = NULL;
	}
	free(cmd);
	to_be_del = NULL;
}

void	free_tree(t_tree **tree)
{
	if (!(*tree))
		return ;
	if ((*tree)->type != AST_CMD)
	{
		free_tree(&(*tree)->left);
		if ((*tree)->right)
			free_tree(&(*tree)->right);
	}
	else if ((*tree)->content)
		free_cmd((*tree)->content);
	free(*tree);
	(*tree) = NULL;
}

void	unlink_heredoc(void *heredoc)
{
	char	*heredoc_filepath;

	heredoc_filepath = (char *)heredoc;
	if (heredoc_filepath == NULL)
		return ;
	unlink(heredoc_filepath);
	free(heredoc);
}

void	free_minishell(t_shell *shell)
{
	shell->index = 0;
	if (shell->token_list)
		ft_lstclear(&shell->token_list, free_token);
	if (shell->env_list)
		ft_lstclear(&shell->env_list, free);
	if (shell->tree_root)
		free_tree(&shell->tree_root);
	if (shell->heredoc)
		ft_lstclear(&shell->heredoc, unlink_heredoc);
	free(shell);
}
