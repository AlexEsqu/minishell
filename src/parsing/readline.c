/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 13:51:38 by mkling            #+#    #+#             */
/*   Updated: 2025/01/30 10:47:44 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_tokens(t_list *node)
{
	while (node != NULL)
	{
		printf("TOKEN %d -", ((t_token *)node->content)->lexem);
		printf(" %c :", ((t_token *)node->content)->letter);
		if (node->content != NULL)
			printf("%s", ((t_token *)node->content)->content);
		printf("\n");
		node = node->next;
	}
}

void	parse_and_exec_cmd(t_shell *shell, char *input)
{
	shell->cmd_line = input;
	shell->critical_er = 0;
	scan(shell, &shell->token_list, input);
	if (check_syntax(shell, shell->token_list) != 0)
		return (ft_lstclear(&shell->token_list, free_token));
	lexer(shell, &shell->token_list);
	parser(shell);
	shell->last_exit_code = exec_tree(shell, shell->tree_root, false);
	free_tree(&shell->tree_root);
}

void	init_readline(t_shell *shell)
{
	char		*input;

	signals();
	while (1)
	{
		input = readline("shell$ ");
		if (!input)
			break ;
		if (input && countword(input, ' ') > 0)
		{
			parse_and_exec_cmd(shell, input);
			add_history(input);
			free(input);
		}
	}
	ft_putstr_fd("exit\n", 1);
	rl_clear_history();
}
