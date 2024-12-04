/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 13:06:39 by alex              #+#    #+#             */
/*   Updated: 2024/12/04 07:50:20 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	get_token_count(t_token *start, t_token *end)
{
	t_token	*current;
	size_t	count;

	current = start;
	count = 0;
	while (current != end)
	{
		count++;
		current = current->next;
	}
	return (count);
}

size_t	count_tokens(t_token *start, int letter)
{
	t_token	*current;
	size_t	count;

	current = start;
	count = 0;
	while (current->lexem != END)
	{
		if (current->letter == letter)
			count++;
		current = current->next;
	}
	return (count);
}

// char	**id_infiles(t_cmd_tab *cmd_tab, t_token *start)
// {
// 	int		index;
// 	int		infile_count;
// 	t_token	*current;
// 	char	**infile_list;

// 	infile_count = count_token(start, '<');
// 	if (infile_count == 0)
// 		return (NULL);
// 	infile_list = ft_calloc(sizeof(char *), infile_count + 1);
// 	if (!infile_list)
// 	{
// 		set_error(MALLOC_FAIL, cmd_tab,
// 			"Failed to allocate infile path");
// 		return (NULL);
// 	}
// 	index = 0;
// 	current = start;
// 	while (index < infile_count)
// 	{
// 		current = find_token_in_list(current, '<');
// 		infile_list[index] = ft_strdup(current->next->content);
// 		current = current->next->next;
// 		index++;
// 	}
// 	infile_list[index] = NULL;
// 	return (infile_list);
// }

char	**create_argv(t_cmd_tab *cmd_tab, t_token *start)
{
	int		index;
	t_token	*current;
	char	**cmd_argv;

	cmd_argv = malloc(sizeof(char *)
			* (get_token_count(start, find_token_in_list(start, '\0')) + 1));
	if (!cmd_argv)
	{
		set_error(MALLOC_FAIL, cmd_tab,
			"Failed to allocate command structure");
		return (NULL);
	}
	current = start;
	index = 0;
	while (current->lexem != END)
	{
		cmd_argv[index] = ft_strdup(current->content);
		current = current->next;
		index++;
	}
	cmd_argv[index] = NULL;
	return (cmd_argv);
}

void	parse_cmd(t_cmd_tab *cmd_tab, t_token *start)
{
	t_cmd	*new_cmd;

	new_cmd = create_cmd(cmd_tab);
	if (!new_cmd)
	{
		set_error(MALLOC_FAIL, cmd_tab,
			"Failed to allocate command structure");
		return ;
	}
	new_cmd->outfile = NULL;
	new_cmd->infile = NULL;
	new_cmd->argv = create_argv(cmd_tab, start);
	cmd_tab->cmd_count++;
}

void	parse(t_cmd_tab *cmd_tab, t_token *start)
{
	t_token	*pipe;

	pipe = find_token_in_list(start, '|');
	if (pipe != NULL)
	{
		parse(cmd_tab, pipe->next);
		pipe->lexem = END;
	}
	if (start->lexem != END)
		parse_cmd(cmd_tab, start);
}
