/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utest.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 15:58:43 by mkling            #+#    #+#             */
/*   Updated: 2024/11/20 19:33:48 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

Test(Parsing, Parsing1)
{
	cr_expect(parse("hello") == 'h', "FAIL");
	cr_expect(parse("no") == 'n');
}

Test(Parsing, Parsing2)
{
	cr_expect(parse("aello") != 'h', "FAIL");
	cr_expect(parse("no") == 'n');
}

Test(Parsing, Parsing3)
{
	cr_expect(parse("aello") != 'h', "FAIL");
	cr_expect(parse("no") == 'n');
}
