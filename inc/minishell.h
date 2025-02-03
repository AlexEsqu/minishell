/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 11:11:25 by mkling            #+#    #+#             */
/*   Updated: 2025/02/03 18:24:46 by mkling           ###   ########.fr       */
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

typedef struct s_token
{
	int				lexem;		// type as determined by the lexer
	char			letter;		// letter or first letter
	char			*content;	// malloced words or letter
}	t_token;

typedef struct s_cmd
{
	char			**argv;		// array created with cmd_path, then arguments
	char			*cmd_path;	// binary filepath, absolute/through PATH
	t_list			*arg_list;	// linked list of command arguments
	int				fd_out;		// fd of the final output redirection
	int				fd_in;		// fd of the final input redirection
	int				exit_code;	// value returned by the execution of command
	int				fork_pid;	// process id of fork sent to execute command
}	t_cmd;

typedef struct s_ast
{
	int				type;		// either pipe, or, and or single cmd node
	void			*content;	// if cmd node, contain pointer to cmd structure
	struct s_ast	*left;		// part of the cmd line left of the operator
	struct s_ast	*right;		// part of the cmd line right of the operator
}	t_tree;

typedef struct s_shell
{
	char		*cmd_line;		// readline return
	t_list		*token_list;	// linked list of tokens id from cmd line
	t_list		*env_list;		// linked list of env strings
	t_tree		*tree_root;		// abstract syntaxic tree
	t_list		*heredoc;		// heredoc to be closed once cmd line is exec
	size_t		index;			// index of command currently being executed
	char		**env;			// env received at start of program
	char		**paths;		// extracted PATH variable of the env
	int			std_out;		// preserved stdout
	int			std_in;			// preserved stdin
	int			critical_er;	// flag if critical error in parent process
	int			last_exit_code;	// result of the last command
}	t_shell;

/* SIGNAL */

void		signals(void);

/* INPUT */

void		parse_and_exec_cmd(t_shell *shell, char *input);
void		extract_env_as_linked_list(t_shell *shell);
void		init_readline(t_shell *shell);

/* TOKENIZER */

void		scan(t_shell *shell, t_list **token_list, char *input);
t_token		*create_token(t_shell *shell, int lexem, char letter, char *content);
void		add_token(t_shell *shell, t_list **dest, char let, char *content);
void		merge_token(t_shell *shell, t_list *start);
void		lexer(t_shell *shell, t_list **token_list);
int			check_syntax(t_shell *shell, t_list *token_list);
void		apply_to_list(t_shell *s, t_list *n, void f(t_shell *, t_list *));
int			letter_is(int lexem, char *string);
int			token_is(int lexem, t_list *node);
void		group_strings(t_shell *shell, t_list *node);
void		remove_delimiter(t_shell *shell, void **ptr_to_string);
void		id_operators(t_shell *shell, t_list *current);

/* EXPANSION */

int			can_expand(t_list *node);
int			has_valid_var(char *string);
int			is_valid_variable(char *input);
void		id_variables(t_shell *shell, t_list *current);

/* PARSER */

t_shell		*create_minishell(char **env);
t_tree		*create_branch(t_shell *shell, int type, void *content);
t_cmd		*create_cmd(void);
void		parser(t_shell *shell);

/* EXECUTION */

int			exec_tree(t_shell *shell, t_tree *tree, bool piped);
int			exec_pipe(t_shell *shell, t_tree *tree);
int			exec_single_cmd(t_shell *shell, t_tree *tree, bool piped);
int			create_fork(t_shell *shell, int	*fork_pid);
void		get_cmd_path(t_shell *shell, t_cmd *cmd);
void		put_arg_in_array(t_cmd *cmd);

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

void		expand(t_shell *shell, t_list *node);
t_list		*find_env(t_list *env_list, char *env_name);
char		**extract_list_as_array(t_shell *shell, t_list *head);
int			replace_env(t_shell *shell, char *env_value);

/* REDIRECTION */

void		redirect_for_cmd(t_shell *shell, t_cmd *cmd);
int			create_pipe(t_shell *shell, int *pipe_fd);
int			connect_pipes_and_exec(t_shell *shell, t_tree *tree,
				int pipe_fd[2], int mode);
void		close_pipe(int *pipe_fd);
void		reset_std(t_shell *shell, bool piped);
void		open_file_and_store_fd_in_cmd(t_shell *shell, t_cmd *cmd, t_list *node);
int			assemble_heredoc(t_shell *shell, t_cmd *cmd, char *end_of_file);
void		unlink_heredoc(void *heredoc);

/* ERROR HANDLING */

void		set_error(int err_code, t_shell *shell);
void		set_cmd_error(int err_code, t_cmd *cmd, char *file_or_cmd);
void		print_error(void);
void		print_syntax_error(t_token *token);

/* CLEAN UP */

void		free_token(void *token);
void		free_cmd(void *cmd);
void		free_tree(t_tree **tree);
void		free_minishell(t_shell *shell);

/* DEBUG */

void		print_tokens(t_list *first);

# define TRUE			1
# define FALSE			0
# define DELIMITERS		"'\"()"
# define OPERATORS		"|><&$"
# define DOLLAR			"$"
# define BLANKS			" \n\t"
# define HEREDOC_LOC	"/home/alex/minishell/heredoc"
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


#endif
