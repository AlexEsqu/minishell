/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utest.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 15:58:43 by mkling            #+#    #+#             */
/*   Updated: 2024/11/21 17:34:03 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include "/home/mkling/Criterion/include/criterion/criterion.h"
#include "/home/mkling/Criterion/include/criterion/redirect.h"



void	redirect_all_stdout(void)
{
	cr_redirect_stdout();
	cr_redirect_stderr();
}

// Test(general, error_message, .init=redirect_all_stdout)
// {
// 	// exit_if(true, "test1", 0, NULL);
// 	fprintf(stderr, "AAA");
// 	cr_assert_stderr_eq_str("AAA");
// }

Test(General, exit_code, .exit_code = 42)
{
	exit_if(true, "No\n", 42, NULL);
}

/* Lexer: Tokenizing */
Test(Lexer, Token_number)
{
	t_cmd_tab	cmd_tab;

	parse("llo", &cmd_tab);
	cr_expect(cmd_tab.content[0] == 'l');
	parse("no", &cmd_tab);
	cr_expect(cmd_tab.content[0] == 'n');
}

/* Parser: */
Test(Parser, Filling)
{
	t_cmd_tab	cmd_tab;

	parse("llo", &cmd_tab);
	cr_expect(cmd_tab.content[0] == 'l');
	parse("no", &cmd_tab);
	cr_expect(cmd_tab.content[0] == 'n');
}
