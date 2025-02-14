/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgodoy <vgodoy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 13:51:38 by mkling            #+#    #+#             */
/*   Updated: 2025/02/14 15:30:16 by vgodoy           ###   ########.fr       */
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

	if (!signals(shell))
		printf("oulala\n");//---------------------
	else
	{
		while (1)
		{
			my_sig_nal = BASE;
			input = readline("shell$ ");
			if (my_sig_nal == CONTROL_C)
			{
				shell->last_exit_code = E_SIG_INT;
				my_sig_nal = BASE;
			}
			if (!input)
				break ;
			if (input && countword(input, ' ') > 0)
			{
				my_sig_nal = TYPING;
				parse_and_exec_cmd(shell, input);
				if (my_sig_nal == CONTROL_C)
					shell->last_exit_code = E_SIG_INT;
				my_sig_nal = BASE;
				add_history(input);
				free(input);
			}
		}
	}
	ft_putstr_fd("exit\n", 1);
	rl_clear_history();
}
