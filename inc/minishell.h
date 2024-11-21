/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 16:16:49 by mkling            #+#    #+#             */
/*   Updated: 2024/11/21 18:44:59 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# include <string.h>
# include "../inc/libft/inc/libft.h"

/* Commands Data structure */
typedef struct s_cmd_table {
	char	*content;
	t_cmd	cmd;
}	t_cmd_tab;

typedef struct s_cmd {
	char	*cmd_path;
	char	**cmd_argv;
	int		cmd_index;
	int		cmd_exit_code;
}	t_cmd;

/* Abstract Syntax Tree */
typedef enum e_type {
	IDENTIFIER,
	LITERAL,
	OPERATOR,
	PIPE,
	SEPARATOR,
	WHITESPACE
}	t_type;

typedef struct s_ast {
	t_type			type;
	int				index;
	char			*content;
	struct s_ast	*first;
	struct s_ast	*left;
	struct s_ast	*right;
}	t_tree;

/* LEXING  */
/* Logical analysis of the input into tokens */
void	lex(char *string, t_tree *tree);

/* PARSING  */
/* Loading tokens into the data structure */
void	parse(char *string, t_cmd_tab *cmd_tab);

/* EXPANDING */
/* Expand variable into their full form */


/* REDIRECTION  */
/* Identifying infiles and outfile for each command */


/* EXECUTION */
/* Creating forks to execute each command */


/* ERROR HANDLING  */
/* Frees structures and close fds if condition is true */
void	exit_if(bool condition, char *err_message, int err_code, void *data);

#endif
