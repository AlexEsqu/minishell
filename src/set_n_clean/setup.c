/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgodoy <vgodoy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 17:25:58 by vgodoy            #+#    #+#             */
/*   Updated: 2025/02/20 19:40:20 by vgodoy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*create minishell by creating std_in std_out
	and extract environment as linked list and array*/
t_shell	*create_minishell(char **env)
{
	t_shell	*shell;

	shell = ft_calloc(sizeof(t_shell), 1);
	if (!shell)
		return (NULL);
	dup2(STDOUT_FILENO, shell->std_out);
	dup2(STDIN_FILENO, shell->std_in);
	shell->env = env;
	extract_env_as_linked_list(shell);
	shell->env = extract_list_as_array(shell, shell->env_list);
	return (shell);
}

/*create token and puts its content in it*/
t_token	*create_token(t_shell *shell, int lexem, char letter, char *content)
{
	t_token	*token;

	token = (t_token *)ft_calloc(sizeof(t_token), 1);
	if (!token)
		return (set_error(MALLOC_FAIL, shell), NULL);
	token->letter = letter;
	token->lexem = lexem;
	if (content)
		token->content = content;
	return (token);
}

/*function creating the necessary files
	check for malloc fail
	assign a file mode and create a file path
	check for permissions with check_file
	if file is to become a heredoc, collects it with assemble heredoc
	puts the file at the back of the list*/
void	create_file(t_shell *shell, t_cmd *cmd, t_token *token)
{
	t_file	*file;
	t_list	*node;

	file = (t_file *)ft_calloc(sizeof(t_file), 1);
	if (!file)
		return (set_error(MALLOC_FAIL, shell));
	file->mode = token->lexem;
	file->path = ft_strdup(token->content);
	if (!file->path)
		return (set_error(MALLOC_FAIL, shell));
	check_file(shell, cmd, file);
	if (file->mode == HEREDOC)
		assemble_heredoc(shell, cmd, file);
	if (my_sig_nal == CONTROL_C)
		return ;
	node = ft_lstnew(file);
	if (!node)
		return (set_error(MALLOC_FAIL, shell));
	ft_lstadd_back(&cmd->files, node);
}

/*create command and initialises values*/
t_cmd	*create_cmd(void)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)ft_calloc(sizeof(t_cmd), 1);
	if (!cmd)
		return (NULL);
	cmd->fork_pid = -1;
	cmd->fd_in = -2;
	cmd->fd_out = -2;
	return (cmd);
}

/*util function to appli a function to a list*/
void	apply_to_list(t_shell *shell, t_list *node,
			void function(t_shell *, t_list *))
{
	while (node != NULL)
	{
		function(shell, node);
		node = node->next;
	}
}
