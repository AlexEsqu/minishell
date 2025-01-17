/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utest.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 15:58:43 by mkling            #+#    #+#             */
/*   Updated: 2025/01/09 15:50:07 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <criterion/new/assert.h>
#include <sys/stat.h>

extern char	**environ;

void	redirect_all_std(void)
{
	cr_redirect_stderr();
	cr_redirect_stdout();
}

/* WARNING : Criterion must be installed and accessible
For installation on school machine, see:
https://github.com/RoKerjea/Criterion-demo-42/blob/main/criterion_install.sh */

/* ************************************************************************** */
/*																			  */
/*	PARSING																	  */
/*																			  */
/* ************************************************************************** */
/* ************************************************************************** */
/*	Scan																	  */
/* ************************************************************************** */

Test(Scan, Empty)
{
	t_shell	shell;
	t_list	*dest = NULL;
	char	*input = "";

	shell.index = 0;
	scan(&shell, &dest, input);
	cr_assert_eq(((t_token *)dest->content)->lexem, START);
	cr_assert_eq(((t_token *)dest->next->content)->lexem, END);
}

Test(Scan, SingleBlank)
{
	t_shell	shell;
	t_list	*dest = NULL;
	char	*input = " ";

	shell.index = 0;
	scan(&shell, &dest, input);

	cr_assert_eq(((t_token *)dest->content)->lexem, START);
	cr_assert_eq(((t_token *)dest->next->content)->lexem, BLANK);
	cr_assert_eq(((t_token *)dest->next->next->content)->lexem, END);
}

Test(Scan, SingleWord)
{
	t_shell	shell;
	t_list	*dest = NULL;
	char	*input = "hello";

	shell.index = 0;
	scan(&shell, &dest, input);

	cr_assert_eq(((t_token *)dest->content)->lexem, START);
	cr_assert_eq(((t_token *)dest->next->content)->lexem, WORD);
	cr_assert_str_eq(((char *)((t_token *)dest->next->content)->content), "hello");
	cr_assert_eq(((t_token *)dest->next->next->content)->lexem, END);
}

Test(Scan, SinglePipe)
{
	t_shell	shell;
	t_list	*dest = NULL;
	char	*input = "|";

	shell.index = 0;
	scan(&shell, &dest, input);

	cr_assert_eq(((t_token *)dest->content)->lexem, START);
	cr_assert_eq(((t_token *)dest->next->content)->lexem, OPERATOR);
	cr_assert_eq((((t_token *)dest->next->content)->letter), '|');
	cr_assert(eq(int, ((t_token *)dest->next->next->content)->lexem, END));
}

Test(Scan, SingleQuote)
{
	t_shell	shell;
	t_list	*dest = NULL;
	char	*input = "\"";

	shell.index = 0;
	scan(&shell, &dest, input);

	cr_assert_eq(((t_token *)dest->content)->lexem, START);
	cr_assert_eq(((t_token *)dest->next->content)->lexem, DELIMITER);
	cr_assert_eq((((t_token *)dest->next->content)->letter), '"');
	cr_assert_eq(((t_token *)dest->next->next->content)->lexem, END);
}

Test(Scan, OpenQuote)
{
	t_shell	shell;
	t_list	*dest = NULL;
	char	*input = "\"   ";

	shell.index = 0;
	scan(&shell, &dest, input);

	cr_assert_eq(((t_token *)dest->content)->lexem, START);
	cr_assert_eq(((t_token *)dest->next->content)->lexem, DELIMITER);
	cr_assert_eq((((t_token *)dest->next->content)->letter), '"');
	cr_assert_eq(((t_token *)dest->next->next->content)->lexem, BLANK);
	cr_assert_eq(((t_token *)dest->next->next->next->content)->lexem, END);
}

Test(Scan, SingleDollar)
{
	t_shell	shell;
	t_list	*dest = NULL;
	char	*input = "$   ";

	shell.index = 0;
	scan(&shell, &dest, input);

	cr_assert_eq(((t_token *)dest->content)->lexem, START);
	cr_assert_eq(((t_token *)dest->next->content)->lexem, WORD);
	cr_assert_eq((((t_token *)dest->next->content)->letter), '$');
	cr_assert_eq(((t_token *)dest->next->next->content)->lexem, BLANK);
	cr_assert_eq(((t_token *)dest->next->next->next->content)->lexem, END);
}

Test(Scan, SingleVariable)
{
	t_shell	shell;
	t_list	*dest = NULL;
	char	*input = "$a   ";

	shell.index = 0;
	scan(&shell, &dest, input);

	cr_assert_eq(((t_token *)dest->content)->lexem, START);
	cr_assert_eq(((t_token *)dest->next->content)->lexem, OPERATOR);
	cr_assert_eq((((t_token *)dest->next->content)->letter), '$');
	cr_assert_eq(((t_token *)dest->next->next->content)->lexem, WORD);
	cr_assert_eq((((t_token *)dest->next->next->content)->letter), 'a');
	cr_assert_eq(((t_token *)dest->next->next->next->content)->lexem, BLANK);
	cr_assert_eq(((t_token *)dest->next->next->next->next->content)->lexem, END);
}

/* ************************************************************************** */
/*	Syntax																	  */
/* ************************************************************************** */

Test(Syntax, Valid_Single_Command0)
{
	t_shell	*shell;

	shell = create_minishell(environ);
	shell->cmd_line = "echo hello";
	scan(shell, &shell->token_list, shell->cmd_line);
	cr_assert(check_syntax(shell, shell->token_list) == 0);
	cr_assert(shell->critical_er == 0);
	free_minishell(shell);
}

Test(Syntax, Valid_Single_Command1)
{
	t_shell	*shell;

	shell = create_minishell(environ);
	shell->cmd_line = "echo      hello     ";
	scan(shell, &shell->token_list, shell->cmd_line);
	cr_assert(check_syntax(shell, shell->token_list) == 0);
	cr_assert(shell->critical_er == 0);
	free_minishell(shell);
}

Test(Syntax, Valid_String)
{
	t_shell	*shell;

	shell = create_minishell(environ);
	shell->cmd_line = "echo   \"hello and goodbye\"";
	scan(shell, &shell->token_list, shell->cmd_line);
	cr_assert(check_syntax(shell, shell->token_list) == 0);
	cr_assert(shell->critical_er == 0);
	free_minishell(shell);
}

Test(Syntax, Valid_Out_Redirection0)
{
	t_shell	*shell;

	shell = create_minishell(environ);
	shell->cmd_line = "echo hello > test.txt";
	scan(shell, &shell->token_list, shell->cmd_line);
	cr_assert(check_syntax(shell, shell->token_list) == 0);
	cr_assert(shell->critical_er == 0);
	free_minishell(shell);
}

Test(Syntax, Valid_Out_Redirection1)
{
	t_shell	*shell;

	shell = create_minishell(environ);
	shell->cmd_line = "echo hello >test.txt";
	scan(shell, &shell->token_list, shell->cmd_line);
	cr_assert(check_syntax(shell, shell->token_list) == 0);
	cr_assert(shell->critical_er == 0);
	free_minishell(shell);
}

Test(Syntax, Valid_In_Redirection0)
{
	t_shell	*shell;

	shell = create_minishell(environ);
	shell->cmd_line = "< Makefile grep a";
	scan(shell, &shell->token_list, shell->cmd_line);
	cr_assert(check_syntax(shell, shell->token_list) == 0);
	cr_assert(shell->critical_er == 0);
	free_minishell(shell);
}

Test(Syntax, Valid_In_Redirection1)
{
	t_shell	*shell;

	shell = create_minishell(environ);
	shell->cmd_line = "<Makefile grep a";
	scan(shell, &shell->token_list, shell->cmd_line);
	cr_assert(check_syntax(shell, shell->token_list) == 0);
	cr_assert(shell->critical_er == 0);
	free_minishell(shell);
}

Test(Syntax, Valid_Redirections0)
{
	t_shell	*shell;

	shell = create_minishell(environ);
	shell->cmd_line = "< Makefile grep a > test.txt";
	scan(shell, &shell->token_list, shell->cmd_line);
	cr_assert(check_syntax(shell, shell->token_list) == 0);
	cr_assert(shell->critical_er == 0);
	free_minishell(shell);
}

Test(Syntax, Valid_Redirections1)
{
	t_shell	*shell;

	shell = create_minishell(environ);
	shell->cmd_line = "<Makefile grep a >test.txt";
	scan(shell, &shell->token_list, shell->cmd_line);
	cr_assert(check_syntax(shell, shell->token_list) == 0);
	cr_assert(shell->critical_er == 0);
	free_minishell(shell);
}

Test(Syntax, Valid_Pipe0)
{
	t_shell	*shell;

	shell = create_minishell(environ);
	shell->cmd_line = "ls -a | wc -l";
	scan(shell, &shell->token_list, shell->cmd_line);
	cr_assert(check_syntax(shell, shell->token_list) == 0);
	cr_assert(shell->critical_er == 0);
	free_minishell(shell);
}

Test(Syntax, Valid_Pipe1)
{
	t_shell	*shell;

	shell = create_minishell(environ);
	shell->cmd_line = "ls -a |wc -l";
	scan(shell, &shell->token_list, shell->cmd_line);
	cr_assert(check_syntax(shell, shell->token_list) == 0);
	cr_assert(shell->critical_er == 0);
	free_minishell(shell);
}

Test(Syntax, Valid_Pipe2)
{
	t_shell	*shell;

	shell = create_minishell(environ);
	shell->cmd_line = "ls -a| wc -l";
	scan(shell, &shell->token_list, shell->cmd_line);
	cr_assert(check_syntax(shell, shell->token_list) == 0);
	cr_assert(shell->critical_er == 0);
	free_minishell(shell);
}

Test(Syntax, Empty_Command)
{
	t_shell	*shell;

	shell = create_minishell(environ);
	shell->cmd_line = "";
	scan(shell, &shell->token_list, shell->cmd_line);
	cr_assert(check_syntax(shell, shell->token_list) == 0);
	cr_assert(shell->critical_er == 0);
	free_minishell(shell);
}

Test(Syntax, Invalid_Pipe0, .init=redirect_all_std)
{
	t_shell	*shell;

	shell = create_minishell(environ);
	shell->cmd_line = "   | echo hello";
	scan(shell, &shell->token_list, shell->cmd_line);
	cr_assert(check_syntax(shell, shell->token_list) != 0);
	cr_assert(shell->critical_er == 1);
	cr_assert_stderr_eq_str("shell: syntax error near unexpected token '|'\n");
}

Test(Syntax, Invalid_Pipe1, .init=redirect_all_std)
{
	t_shell	*shell;

	shell = create_minishell(environ);
	shell->cmd_line = "echo hello|";
	scan(shell, &shell->token_list, shell->cmd_line);
	cr_assert(check_syntax(shell, shell->token_list) != 0);
	cr_assert(shell->critical_er == 1);
	cr_assert_stderr_eq_str("shell: syntax error near unexpected token '|'\n");
}

Test(Syntax, Invalid_Redirection1, .init=redirect_all_std)
{
	t_shell	*shell;

	shell = create_minishell(environ);
	shell->cmd_line = "<";
	scan(shell, &shell->token_list, shell->cmd_line);
	cr_assert(check_syntax(shell, shell->token_list) != 0);
	cr_assert(shell->critical_er == 1);
	cr_assert_stderr_eq_str("shell: syntax error near unexpected token 'newline'\n");
	free_minishell(shell);
}

Test(Syntax, Invalid_Redirection2, .init=redirect_all_std)
{
	t_shell	*shell;

	shell = create_minishell(environ);
	shell->cmd_line = "> ";
	scan(shell, &shell->token_list, shell->cmd_line);
	cr_assert(check_syntax(shell, shell->token_list) != 0);
	cr_assert(shell->critical_er == 1);
	cr_assert_stderr_eq_str("shell: syntax error near unexpected token 'newline'\n");
	free_minishell(shell);
}

/* ************************************************************************** */
/*	Lexer																	  */
/* ************************************************************************** */

Test(Lexer, Remove_single_space)
{
	t_shell	*shell;

	shell = create_minishell(environ);
	shell->cmd_line = "echp hello";
	scan(shell, &shell->token_list, shell->cmd_line);
	lexer(shell, &shell->token_list);
	cr_assert_eq(((t_token *)shell->token_list->content)->lexem, START);
	cr_assert_eq(((t_token *)shell->token_list->next->content)->lexem, WORD);
	cr_assert_str_eq((char *)(((t_token *)shell->token_list->next->content)->content), "echp");
	cr_assert_eq(((t_token *)shell->token_list->next->next->content)->lexem, WORD);
	cr_assert_str_eq((char *)(((t_token *)shell->token_list->next->next->content)->content), "hello");
	cr_assert_eq(((t_token *)shell->token_list->next->next->next->content)->lexem, END);
}

Test(Lexer, Remove_many_spaces)
{
	t_shell	*shell;

	shell = create_minishell(environ);
	shell->cmd_line = "echp       hello";
	scan(shell, &shell->token_list, shell->cmd_line);
	lexer(shell, &shell->token_list);
	cr_assert_eq(((t_token *)shell->token_list->content)->lexem, START);
	cr_assert_eq(((t_token *)shell->token_list->next->content)->lexem, WORD);
	cr_assert_str_eq((char *)(((t_token *)shell->token_list->next->content)->content), "echp");
	cr_assert_eq(((t_token *)shell->token_list->next->next->content)->lexem, WORD);
	cr_assert_str_eq((char *)(((t_token *)shell->token_list->next->next->content)->content), "hello");
	cr_assert_eq(((t_token *)shell->token_list->next->next->next->content)->lexem, END);
}

Test(Lexer, Id_variable)
{
	t_shell	*shell;
	t_token	*token;

	shell = create_minishell(environ);
	shell->cmd_line = "echp       $hello";
	scan(shell, &shell->token_list, shell->cmd_line);
	lexer(shell, &shell->token_list);
	token = (t_token *)shell->token_list->content;
	cr_assert_eq(token->lexem, START);
	token = (t_token *)shell->token_list->next->content;
	cr_assert_eq(token->lexem, WORD);
	cr_assert_str_eq(((char *)token->content), "echp");
	token = (t_token *)shell->token_list->next->next->content;
	cr_assert_eq(token->lexem, VARIABLE);
	cr_assert_eq(token->letter, '$');
	cr_assert_str_eq((char *)token->content, "$hello");
	token = (t_token *)shell->token_list->next->next->next->content;
	cr_assert_eq(token->lexem, END);
}

Test(Lexer, Id_multiple_variable)
{
	t_shell	*shell;
	t_token	*token;

	shell = create_minishell(environ);
	shell->cmd_line = "echp       $hello$arg";
	scan(shell, &shell->token_list, shell->cmd_line);
	lexer(shell, &shell->token_list);
	token = (t_token *)shell->token_list->content;
	cr_assert_eq(token->lexem, START);
	token = (t_token *)shell->token_list->next->content;
	cr_assert_eq(token->lexem, WORD);
	cr_assert_str_eq(((char *)token->content), "echp");
	token = (t_token *)shell->token_list->next->next->content;
	cr_assert_eq(token->lexem, VARIABLE);
	cr_assert_eq(token->letter, '$');
	cr_assert_str_eq((char *)token->content, "$hello");
	token = (t_token *)shell->token_list->next->next->next->content;
	cr_assert_eq(token->lexem, VARIABLE);
	cr_assert_eq(token->letter, '$');
	cr_assert_str_eq((char *)token->content, "$arg");
	token = (t_token *)shell->token_list->next->next->next->next->content;
	cr_assert_eq(token->lexem, END);
}

Test(Lexer, Wrong_variable_number)
{
	t_shell	*shell;
	t_token	*token;

	shell = create_minishell(environ);
	shell->cmd_line = "echp       $1";
	scan(shell, &shell->token_list, shell->cmd_line);
	lexer(shell, &shell->token_list);
	token = (t_token *)shell->token_list->content;
	cr_assert_eq(token->lexem, START);
	token = (t_token *)shell->token_list->next->content;
	cr_assert_eq(token->lexem, WORD);
	cr_assert_str_eq(((char *)token->content), "echp");
	token = (t_token *)shell->token_list->next->next->content;
	cr_assert_eq(token->lexem, VARIABLE);
	cr_assert_eq(token->letter, '$');
	cr_assert_str_eq((char *)token->content, "$1");
	token = (t_token *)shell->token_list->next->next->next->content;
	cr_assert_eq(token->lexem, END);
}



/* ************************************************************************** */
/*	Parsing																	  */
/* ************************************************************************** */






/* ************************************************************************** */
/*	Expand																	  */
/* ************************************************************************** */

Test(Expand, valid_variable)
{
	t_list	env_node = {
		.next = NULL,
		.prev = NULL,
		.content = ft_strdup("USERR=alex"),
	};
	t_list	arg_node = {
		.next = NULL,
		.prev = NULL,
		.content = ft_strdup("$USERR"),
	};
	t_shell	shell = {
		.last_exit_code = 0,
		.env_list = &env_node,
	};

	expand(&shell, &arg_node);
	cr_assert_str_eq(arg_node.content, "alex");
}

Test(Expand, absent_variable)
{
	t_list	env_node = {
		.next = NULL,
		.prev = NULL,
		.content = ft_strdup("USER=alex"),
	};
	t_list	arg_node = {
		.next = NULL,
		.prev = NULL,
		.content = ft_strdup("$INVALID"),
	};
	t_shell	shell = {
		.last_exit_code = 0,
		.env_list = &env_node,
	};

	expand(&shell, &arg_node);
	cr_assert(arg_node.content == NULL);
}

Test(Expand, invalid_variable)
{
	t_list	env_node = {
		.next = NULL,
		.prev = NULL,
		.content = ft_strdup("USER=alex"),
	};
	t_list	arg_node = {
		.next = NULL,
		.prev = NULL,
		.content = ft_strdup("$"),
	};
	t_shell	shell = {
		.last_exit_code = 0,
		.env_list = &env_node,
	};

	expand(&shell, &arg_node);
	cr_assert_str_eq(arg_node.content, "$");
}

Test(Expand, exit_code_variable)
{
	t_list	env_node = {
		.next = NULL,
		.prev = NULL,
		.content = ft_strdup("USER=alex"),
	};
	t_list	arg_node = {
		.next = NULL,
		.prev = NULL,
		.content = ft_strdup("$?"),
	};
	t_shell	shell = {
		.last_exit_code = 42,
		.env_list = &env_node,
	};

	expand(&shell, &arg_node);
	cr_assert_str_eq(arg_node.content, "42");
}

Test(Expand, string_with_variable)
{
	t_list	env_node = {
		.next = NULL,
		.prev = NULL,
		.content = ft_strdup("USER=alex"),
	};
	t_list	arg_node = {
		.next = NULL,
		.prev = NULL,
		.content = ft_strdup("hello $USER"),
	};
	t_shell	shell = {
		.last_exit_code = 42,
		.env_list = &env_node,
	};

	expand(&shell, &arg_node);
	cr_assert_str_eq(arg_node.content, "hello alex");
}

Test(Expand, string_without_variable)
{
	t_list	env_node = {
		.next = NULL,
		.prev = NULL,
		.content = ft_strdup("USER=alex"),
	};
	t_list	arg_node = {
		.next = NULL,
		.prev = NULL,
		.content = ft_strdup("hello and goodbye"),
	};
	t_shell	shell = {
		.last_exit_code = 42,
		.env_list = &env_node,
	};

	expand(&shell, &arg_node);
	cr_assert_str_eq(arg_node.content, "hello and goodbye");
}

Test(Expand, string_with_invalid_variable)
{
	t_list	env_node = {
		.next = NULL,
		.prev = NULL,
		.content = ft_strdup("USER=alex"),
	};
	t_list	arg_node = {
		.next = NULL,
		.prev = NULL,
		.content = ft_strdup("hello and $ goodbye"),
	};
	t_shell	shell = {
		.last_exit_code = 0,
		.env_list = &env_node,
	};

	expand(&shell, &arg_node);
	cr_assert_str_eq(arg_node.content, "hello and $ goodbye");
}



/* ************************************************************************** */
/*																			  */
/*	EXECUTION																  */
/*																			  */
/* ************************************************************************** */
/* ************************************************************************** */
/*	Redirection																	  */
/* ************************************************************************** */

Test(Redirection, get_valid_cmd_path)
{
	t_shell	*shell;
	t_cmd	*cmd;

	shell = create_minishell(environ);
	extract_env_as_linked_list(shell);
	cmd = create_cmd();
	cmd->arg_list = ft_lstnew("ls");
	get_cmd_path(shell, cmd);
	cr_assert(cmd->cmd_path != NULL);
	cr_assert(eq(str, cmd->cmd_path, "/usr/bin/ls"));
}

Test(Redirection, set_infile_forbidden, .init=redirect_all_std)
{
	t_shell	shell;
	t_cmd	cmd;
	t_file	file;
	t_list	node;
	int		fd;

	shell.critical_er = 0;
	fd = open("test/forbidden1.txt", O_CREAT);
	close(fd);
	chmod("test/forbidden1.txt", 0002);
	file.path = "test/forbidden1.txt";
	file.mode = INFILE;
	node.content = &file;
	node.next = NULL;
	node.prev = NULL;
	cmd.infiles = &node;
	cmd.arg_list = ft_lstnew("echo");
	cmd.fd_in = -1;

	set_infile_fd(&shell, &cmd);
	cr_assert(eq(int, cmd.fd_in, -1));
	cr_assert_stderr_eq_str("shell: echo: Forbidden file\n");
	chmod("test/forbidden1.txt", 0777);
	unlink("test/forbidden1.txt");
}

Test(Redirection, set_outfile_forbidden, .init=redirect_all_std)
{
	t_cmd	cmd;
	t_file	file;
	t_list	node;
	int		fd;

	fd = open("test/forbidden.txt", O_CREAT);
	close(fd);
	chmod("test/forbidden.txt", 0004);
	file.path = "test/forbidden.txt";
	file.mode = OUTFILE;
	node.content = &file;
	node.next = NULL;
	node.prev = NULL;
	cmd.outfiles = &node;
	cmd.arg_list = ft_lstnew("echo");
	cmd.fd_out = -1;

	set_outfile_fd(&cmd);
	cr_assert(eq(int, cmd.fd_out, -1));
	cr_assert_stderr_eq_str("shell: echo: Forbidden file\n");
	chmod("test/forbidden.txt", 0777);
	unlink("test/forbidden.txt");
}

Test(Redirection, no_path_relative)
{
	t_shell	*shell;
	t_cmd	*cmd;
	t_list	*path_env;

	shell = create_minishell(environ);
	extract_env_as_linked_list(shell);
	cmd = create_cmd();
	ft_lstadd_back(&cmd->arg_list, ft_lstnew("ls"));
	path_env = find_env(shell->env_list, "PATH");
	ft_lstpop(&shell->env_list, path_env, free);
	get_cmd_path(shell, cmd);
	cr_assert(cmd->exit_code = 127);
	cr_assert_stderr_eq_str("shell: ls: No PATH variable found\n");
	free_minishell(shell);
}

/* ************************************************************************** */
/*	Path																	  */
/* ************************************************************************** */

Test(Path, get_path_relative_command_with_dot)
{
	t_cmd	*cmd;
	t_shell	*shell;

	shell = create_minishell(environ);
	cmd = create_cmd();
	cmd->arg_list = ft_lstnew("./test/allowed");
	get_cmd_path(shell, cmd);
	cr_assert(cmd->exit_code == 0);
	cr_assert_str_eq(cmd->cmd_path, "./test/allowed");
}

Test(Path, get_path_relative_command)
{
	t_cmd	*cmd;
	t_shell	*shell;

	shell = create_minishell(environ);
	cmd = create_cmd();
	cmd->arg_list = ft_lstnew("test/allowed");
	get_cmd_path(shell, cmd);
	cr_assert(cmd->exit_code == 0);
	cr_assert_str_eq(cmd->cmd_path, "test/allowed");
}

Test(Path, get_path_unknown_command, .init=redirect_all_std)
{
	t_cmd	*cmd;
	t_shell	*shell;

	shell = create_minishell(environ);
	cmd = create_cmd();
	cmd->arg_list = ft_lstnew("eccho");
	get_cmd_path(shell, cmd);
	cr_assert(cmd->exit_code == 127);
	cr_assert_stderr_eq_str("shell: eccho: Command not found\n");
}

Test(Path, get_path_forbidden_command, .init=redirect_all_std)
{
	t_cmd	*cmd;
	int		fd;
	t_shell	*shell;

	shell = create_minishell(environ);
	cmd = create_cmd();
	fd = open("test/forbidden", O_CREAT);
	close(fd);
	chmod("test/forbidden", 0004);
	cmd->arg_list = ft_lstnew("./test/forbidden");
	get_cmd_path(shell, cmd);
	cr_assert(cmd->exit_code == 126);
	cr_assert_stderr_eq_str("shell: ./test/forbidden: Permission denied\n");
	unlink("test/forbidden");
}

Test(Path, get_path_directory_cmd, .init=redirect_all_std)
{
	t_cmd	*cmd;
	t_shell	*shell;

	shell = create_minishell(environ);
	cmd = create_cmd();
	cmd->arg_list = ft_lstnew("test/");
	get_cmd_path(shell, cmd);
	cr_assert(cmd->exit_code == 126);
	cr_assert_stderr_eq_str("shell: test/: Is a directory\n");
}

Test(Path, get_path_wrong_directory_cmd, .init=redirect_all_std)
{
	t_cmd	*cmd;
	t_shell	*shell;

	shell = create_minishell(environ);
	cmd = create_cmd();
	cmd->arg_list = ft_lstnew("test/utest.c/");
	get_cmd_path(shell, cmd);
	cr_assert(cmd->exit_code == 126);
	cr_assert_stderr_eq_str("shell: test/utest.c/: Is not a directory\n");
}

/* ************************************************************************** */
/*	Execution																  */
/* ************************************************************************** */

Test(Execution, relative_command, .init=redirect_all_std)
{
	t_shell	*shell;

	shell = create_minishell(environ);
	shell->last_exit_code = -1;
	shell->cmd_line = "./test/allowed";
	parse_and_exec_cmd(shell, shell->cmd_line);
	cr_assert(shell->last_exit_code == 0);
	cr_assert_stdout_eq_str("allowed\n");
	cr_assert(shell->last_exit_code == 0);
	free_minishell(shell);
}

/* ************************************************************************** */
/*	Pipe																	  */
/* ************************************************************************** */

Test(Pipe, pipe_echo_cat_e, .init=redirect_all_std)
{
	t_shell *shell;

	shell = create_minishell(environ);
	shell->last_exit_code = -1;
	shell->cmd_line = "echo hello | cat -e";
	parse_and_exec_cmd(shell, shell->cmd_line);
	cr_assert(shell->last_exit_code == 0);
	cr_assert_stdout_eq_str("hello$\n");
	free_minishell(shell);
}

Test(Pipe, pipe_ls_wc, .init=redirect_all_std)
{
	t_shell *shell;

	shell = create_minishell(environ);
	shell->last_exit_code = -1;
	shell->cmd_line = "ls | wc -l";
	parse_and_exec_cmd(shell, shell->cmd_line);
	cr_assert(shell->last_exit_code == 0);
	cr_assert_stdout_eq_str("7\n");
	free_minishell(shell);
}

Test(Pipe, pipe_yes_head, .init=redirect_all_std)
{
	t_shell *shell;

	shell = create_minishell(environ);
	shell->last_exit_code = -1;
	shell->cmd_line = "yes | head -n 2";
	parse_and_exec_cmd(shell, shell->cmd_line);
	cr_assert(shell->last_exit_code == 0);
	cr_assert_stdout_eq_str("y\ny\n");
	free_minishell(shell);
}

/* ************************************************************************** */
/*																			  */
/*	BUILTINS																  */
/*																			  */
/* ************************************************************************** */
/* ************************************************************************** */
/*	Echo																	  */
/* ************************************************************************** */

Test(Builtin, echo_single, .init=redirect_all_std)
{
	char	*argv[] = {"echo", "hello", NULL};
	int		exit_code;

	exit_code = echo(argv, STDOUT_FILENO);
	cr_assert(eq(int, exit_code, 0));
	cr_assert_stdout_eq_str("hello\n");
}

Test(Builtin, echo_string, .init=redirect_all_std)
{
	char	*argv[] = {"echo", "hello and goodbye", NULL};
	int		exit_code;

	exit_code = echo(argv, STDOUT_FILENO);
	cr_assert(eq(int, exit_code, 0));
	cr_assert_stdout_eq_str("hello and goodbye\n");
}

Test(Builtin, echo_multiple, .init=redirect_all_std)
{
	char	*argv[] = {"echo", "hello", "and", "goodbye", NULL};
	int		exit_code;

	exit_code = echo(argv, STDOUT_FILENO);
	cr_assert(eq(int, exit_code, 0));
	cr_assert_stdout_eq_str("hello and goodbye\n");
}

Test(Builtin, echo_option_single, .init=redirect_all_std)
{
	char	*argv[] = {"echo", "-n", "hello", NULL};
	int		exit_code;

	exit_code = echo(argv, STDOUT_FILENO);
	cr_assert(eq(int, exit_code, 0));
	cr_assert_stdout_eq_str("hello");
}

Test(Builtin, echo_option_string, .init=redirect_all_std)
{
	char	*argv[] = {"echo", "-n", "hello and goodbye", NULL};
	int		exit_code;

	exit_code = echo(argv, STDOUT_FILENO);
	cr_assert(eq(int, exit_code, 0));
	cr_assert_stdout_eq_str("hello and goodbye");
}
