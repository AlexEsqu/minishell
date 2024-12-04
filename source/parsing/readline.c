/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 13:51:38 by mkling            #+#    #+#             */
/*   Updated: 2024/12/04 07:56:41 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_tokens(t_token *first)
{
	t_token	*current;

	current = first;
	while (current)
	{
		printf("TOKEN %d :", current->lexem);
		if (current->content != NULL)
			printf("%s", current->content);
		printf("\n");
		current = current->next;
	}
}

void	init_readline(char **envp)
{
	char		*input;
	t_cmd_tab	*cmd_tab;

	signals();
	while (1)
	{
		input = readline("Minishell> ");
		if (!input)
			break ;
		if (input)
		{
			add_history(input);
			cmd_tab = create_cmd_tab(envp);
			cmd_tab->cmd_line = input;
			lexer(cmd_tab);
			free(input);
			parse(cmd_tab, cmd_tab->token_list);
			free_token_list(cmd_tab);
			if (cmd_tab->cmd_count > 0)
			{
				execute_all_cmd(cmd_tab);
				free_cmd_list(cmd_tab);
			}
			free_cmd_tab(cmd_tab);
		}
	}
}


// theory why more than one command quits readline
// > readline reads a EOF because the stdin is closed ?
