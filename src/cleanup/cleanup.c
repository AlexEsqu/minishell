/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 00:22:21 by alex              #+#    #+#             */
/*   Updated: 2025/02/14 11:41:32 by mkling           ###   ########.fr       */
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

void	free_file(void *to_be_del)
{
	t_file	*file;

	file = (t_file *)to_be_del;
	if (file == NULL)
		return ;
	if (file->delim != NULL)
	{
		free(file->delim);
		unlink(file->path);
	}
	if (file->path != NULL)
		free(file->path);
	free(file);
}

void	free_cmd(void *to_be_del)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)to_be_del;
	if (!cmd)
		return ;
	close_cmd_fd(cmd);
	if (cmd->argv)
		free(cmd->argv);
	if (cmd->cmd_path)
		free(cmd->cmd_path);
	if (cmd->arg_list)
		ft_lstclear(&cmd->arg_list, free);
	if (cmd->files)
		ft_lstclear(&cmd->files, free_file);
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

void	free_minishell(t_shell *shell)
{
	shell->index = 0;
	if (shell->token_list)
		ft_lstclear(&shell->token_list, free_token);
	if (shell->env_list)
		ft_lstclear(&shell->env_list, free);
	if (shell->tree_root)
		free_tree(&shell->tree_root);
	free(shell);
}
