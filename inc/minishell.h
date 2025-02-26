/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 23:24:18 by vgodoy            #+#    #+#             */
/*   Updated: 2025/02/26 13:37:56 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <stdbool.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/inc/libft.h"
# include <errno.h>

extern int	g_my_sig;

typedef struct s_token
{
	int				lexem;
	char			letter;
	char			*content;
}	t_token;

typedef struct s_files
{
	int				mode;
	char			*path;
	char			*delim;
	int				fd;
	bool			is_quoted;
}	t_file;

typedef struct s_cmd
{
	char			**argv;
	int				argc;
	char			*cmd_path;
	t_list			*arg_list;
	t_list			*files;
	int				fd_out;
	int				fd_in;
	int				exit_code;
	int				fork_pid;
}	t_cmd;

typedef struct s_ast
{
	int				type;
	void			*content;
	struct s_ast	*left;
	struct s_ast	*right;
}	t_tree;

typedef struct s_shell
{
	char		*cmd_line;
	t_list		*token_list;
	t_list		*env_list;
	t_tree		*tree_root;
	t_list		*heredoc;
	size_t		index;
	char		**env;
	char		**paths;
	int			std_out;
	int			std_in;
	int			critical_er;
	int			last_exit_code;
}	t_shell;

/* SIGNAL */

void		signals(t_shell *shell, int mode);
void		normal_mode(int signal, siginfo_t *info, void *context);
void		interactive_mode(int signal, siginfo_t *info, void *context);
void		heredoc_mode(int signal, siginfo_t *info, void *context);

/* INPUT */

void		parse_and_exec_cmd(t_shell *shell, char *input);
void		extract_env_as_linked_list(t_shell *shell);
void		init_readline(t_shell *shell);

/* TOKENIZER */

void		scan(t_shell *shell, t_list **token_list, char *input);
t_token		*create_token(t_shell *s, int lexem, char letter, char *content);
void		add_token(t_shell *shell, t_list **dest, char let, char *content);
void		merge_token(t_shell *shell, t_list *start);
void		lexer(t_shell *shell, t_list **token_list);
int			check_syntax(t_shell *shell, t_list *token_list);
void		apply_to_list(t_shell *s, t_list *n, void f(t_shell *, t_list *));
int			letter_is(int lexem, char *string);
int			token_is(int lexem, t_list *node);
void		group_strings(t_shell *shell, t_list *node);
void		remove_quotes_from_string(t_shell *shell, char **ptr_to_string);
void		id_operators(t_shell *shell, t_list *current);
int			count_char_in_string(char *string, int c);
void		remove_parenthesis_from_string(t_shell *shell, char **ptr_str);
void		group_strings(t_shell *shell, t_list *node);

/* EXPANSION */

int			can_expand(t_list *node);
int			has_valid_var(char *string);
int			is_valid_variable(char *input);
void		id_variables(t_shell *shell, t_list *current);
void		expand_string(t_shell *shell, char **ptr_to_str);
t_list		*tokenize_and_expand_string(t_shell *shell, char *string);
void		expand_variable(t_shell *shell, char **ptr_to_variable);
void		glue_words_to_strings(t_shell *shell, t_list *node);

/* PARSER */

t_shell		*create_minishell(char **env);
t_tree		*create_branch(t_shell *shell, int type, void *content);
t_cmd		*create_cmd(void);
void		parser(t_shell *shell);
t_tree		*parse_command(t_shell *shell, t_list **node);

/* EXECUTION */

int			exec_tree(t_shell *shell, t_tree *tree, bool piped);
int			exec_single_cmd(t_shell *shell, t_tree *tree, bool piped);
int			create_fork(t_shell *shell, int	*fork_pid);
void		find_cmd_path(t_shell *shell, t_cmd *cmd);
void		put_arg_in_array(t_cmd *cmd);
int			exec_subshell(t_shell *shell, char *input);

/* BUILT IN */

int			cd(t_shell *shell, t_cmd *cmd);
int			echo(t_cmd *cmd);
int			env(t_shell *shell);
int			export(t_shell *shell, t_cmd *cmd);
int			unset(t_shell *shell, t_cmd *cmd);
int			pwd(t_shell *shell, t_cmd *cmd);
int			exit_shell(t_shell *shell, t_cmd *cmd);
int			exec_builtin(t_shell *shell, t_cmd *cmd);
int			is_builtin(t_cmd *cmd);

/* ENVIRON */

void		expand_node(t_shell *shell, t_list *node);
t_list		*find_env(t_shell *shell, t_list *env_list, char *env_name);
char		**extract_list_as_array(t_shell *shell, t_list *head);
int			replace_env(t_shell *shell, char *env_value);
char		*extract_env_key(t_shell *shell, char *env_key_and_value);
char		*extract_env_value(t_shell *shell, char *env_key_and_value);

/* SIGNAL */

int			delim_summoned(char *line, t_file *file);
int			control_c_pressed(char *line, t_shell *shell);
int			control_d_pressed(char *line, t_file *file);

/* REDIRECTION */

void		open_file(t_cmd *cmd, int mode, char *path);
void		redirect_for_cmd(t_shell *shell, t_cmd *cmd);
int			exec_pipe_monitor(t_shell *shell, t_tree *tree);
void		create_file(t_shell *shell, t_cmd *cmd, t_token *token);
void		assemble_heredoc(t_shell *s, t_cmd *cmd, t_file *file);
void		close_cmd_fd(t_cmd *cmd);
void		check_file(t_shell *shell, t_cmd *cmd, t_file *file);

/* ERROR HANDLING */

void		set_error(int err_code, t_shell *shell);
void		set_cmd_error(int err_code, t_cmd *cmd, char *file_or_cmd);
void		print_error(void);
void		print_syntax_error(t_shell *shell, t_token *token);
char		*get_error_message(int err_code);
int			is_missing_delimiter(t_shell *shell, char *input);
int			input_contains_unsupported(t_shell *shell, char *input);
int			redirection_after_pipe_or_and(t_list *first, t_list *second);

/* CLEAN UP */

void		free_token(void *token);
void		free_cmd(void *cmd);
void		free_tree(t_tree **tree);
void		free_minishell(t_shell *shell);

/* DEBUG */

void		print_tokens(t_list *first);

/* READABILITY */

int			token_is_redirection(t_list *token_node);
int			token_is_operator(t_list *token_node);

# define SHELL_PROMPT "algo$ "
# define TRUE			1
# define FALSE			0
# define DELIMITERS		"'\"()"
# define OPERATORS		"|><&"
# define BLANKS			" \n\t"
# define HEREDOC_LOC	"/tmp/.heredoc_"
# define SHELL_NAME		"shell"
# define PATH_MAX		4096

enum e_lexem
{
	NONE		= 0,
	WORD		= 1,
	BLANK		= 2,
	DELIMITER	= 3,
	OPERATOR	= 4,
	START		= 5,
	END			= 6,
	VARIABLE,
	CMD,
	OUTFILE,
	INFILE		= 10,
	HEREDOC,
	APPEND,
	STRING,
	DOUB_QUOTE,
	SING_QUOTE,
	PIPE,
	GREATER,
	LESSER,
	AND,
	OR			= 20,
	SUBSHELL,
	DOLLAR,
	OPEN_PARENTH,
	CLOS_PARENTH,
};

/* Internal values used inside minishell to print correct error
message or execute correct line of code */
enum e_err_code
{
	SUCCESS = 0,
	GENERAL_ERROR,
	MALLOC_FAIL,
	PIPE_ERROR,
	FORK_ERROR,
	DUP_ERROR,
	NO_FILE,
	NO_CMD,
	READ_ERROR,
	OPEN_ERROR,
	PERM_ERROR,
	SYNTAX_ERROR,
	AST_ERROR,
	PATH_ERROR,
	IS_DIR,
	IS_NOT_DIR,
	TOO_MANY_ARGS,
	TOO_FEW_ARGS,
	SIGNALS,
	INTERUPT,
	AMBIG_REDIR,
	NON_NUM,
	NO_HOME,
	NO_PATH,
	INVALID_VAR,
};

/* Actual return values expected from minishell program */
enum e_exit_code
{
	E_CMD_FAIL = 1,
	E_SYNTAX = 2,
	E_NO_PERM = 126,
	E_NO_CMD = 127,
	E_BAD_EXIT = 128,
	E_SIG_INT = 130,
	E_SIG_SLSH = 131,
};

enum e_pipe_fd
{
	READ = 0,
	WRITE = 1,
};

enum e_ast
{
	AST_PIPE,
	AST_CMD,
	AST_REDIRECTION,
	AST_AND,
	AST_OR,
	AST_SUB,
};

enum e_tree_mode
{
	NO_PIPE,
	IS_PIPE,
	AST_LEFT,
	AST_RIGHT,
};

enum e_my_signal
{
	BASE = 0,
	TYPING = 1,
	IN_HEREDOC = 2,
	CONTROL_C = 3,
	CONTROL_D = 4,
};

enum e_ignal_mode
{
	NORMAL_MODE = 0,
	INTERACTIVE_MODE = 1,
	HEREDOC_MODE = 2,
};

#endif
