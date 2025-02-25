/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/02/25 21:00:49 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

/*test function for debud, print tokens*/
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

/*small function to set up last_exit_code according to the signal*/
void	signal_test(t_shell *shell)
{
	if (my_sig_nal == CONTROL_C)
		shell->last_exit_code = E_SIG_INT;
	if (my_sig_nal == CONTROL_D)
		shell->last_exit_code = E_SIG_SLSH;
	my_sig_nal = BASE;
}

/*  INIT cm_line and critical_er
	SCAN function to create token list
	LEXER funtion to give each token the right control values
	PARSER function to create the tree of commands
	EXEC_TREE fuction to execute the commands*/
void	parse_and_exec_cmd(t_shell *shell, char *input)
{
	shell->cmd_line = input;
	shell->critical_er = 0;
	if (input_contains_unsupported(shell, input))
		return ;
	scan(shell, &shell->token_list, input);
	if (check_syntax(shell, shell->token_list) != 0)
		return (ft_lstclear(&shell->token_list, free_token));
	lexer(shell, &shell->token_list);
	parser(shell);
	if (my_sig_nal != CONTROL_C)
		shell->last_exit_code = exec_tree(shell, shell->tree_root, false);
	free_tree(&shell->tree_root);
}


/*interactive mode of the shell with readline
ctrl+C is detected by signal_test
ctrl+D is detected by (!input) -> exit shell and clear history
if something is writen and entre pushed, enters parse_and_exec_cmd*/
void	init_readline(t_shell *shell)
{
	char		*input;

	while (1)
	{
		my_sig_nal = BASE;
		signals(shell, INTERACTIVE_MODE);
		input = readline(SHELL_PROMPT);
		signals(shell, NORMAL_MODE);
		signal_test(shell);
		if (!input)
			break ;
		if (input && countword(input, ' ') > 0)
		{
			my_sig_nal = TYPING;
			parse_and_exec_cmd(shell, input);
			signal_test(shell);
			add_history(input);
			free(input);
		}
	}
	ft_putstr_fd("exit\n", 1);
	rl_clear_history();
}
