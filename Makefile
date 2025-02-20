# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alex <alex@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/14 14:56:12 by mkling            #+#    #+#              #
#    Updated: 2025/02/20 08:59:40 by alex             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
#		General variables													   #
# **************************************************************************** #

NAME		= minishell

DIR_SRC		= src
DIR_EXEC	= execution
DIR_PARS	= parsing
DIR_BUTI	= builtins
DIR_REDI	= redirection
DIR_CLEA	= cleanup
DIR_EXPA	= expand
DIR_INPU	= input
DIR_LEXR	= lexer

DIR_OBJ		= obj
DIR_OBJS	= $(DIR_OBJ) $(DIR_OBJ)/$(DIR_EXEC) $(DIR_OBJ)/$(DIR_PARS) \
				$(DIR_OBJ)/$(DIR_BUTI) $(DIR_OBJ)/$(DIR_REDI) \
				$(DIR_OBJ)/$(DIR_CLEA) $(DIR_OBJ)/$(DIR_EXPA) \
				$(DIR_OBJ)/$(DIR_INPU) $(DIR_OBJ)/$(DIR_LEXR)

DIR_INC		= inc

DIR_LIB		= $(DIR_INC)/libft

HEADER		= $(DIR_INC)/minishell.h

FUNC_BUTI	=	cd.c \
				echo.c \
				env.c \
				exit.c \
				export.c \
				pwd.c \
				unset.c \
				builtins.c

FUNC_CLEA	=	cleanup.c \
				errors.c \
				messages.c \
				setup.c

FUNC_EXEC	=	execution.c \
				pipe.c \
				ast.c \
				arguments.c

FUNC_EXPA	=	expand.c \
				variable.c \
				delimiters.c \
				tokenizer.c

FUNC_INPU	=	readline.c \
				signals.c \
				modes.c

FUNC_LEXR	=	lexer.c \
				scanner.c \
				syntax.c \
				token.c

FUNC_PARS	=	command.c \
				parser.c \
				path.c \

FUNC_REDI	=	heredoc.c \
				interruptions.c \
				redirection.c

FUNC		=	$(addprefix $(DIR_BUTI)/, $(FUNC_BUTI)) \
				$(addprefix $(DIR_CLEA)/, $(FUNC_CLEA)) \
				$(addprefix $(DIR_EXEC)/, $(FUNC_EXEC)) \
				$(addprefix $(DIR_EXPA)/, $(FUNC_EXPA)) \
				$(addprefix $(DIR_INPU)/, $(FUNC_INPU)) \
				$(addprefix $(DIR_LEXR)/, $(FUNC_LEXR)) \
				$(addprefix $(DIR_PARS)/, $(FUNC_PARS)) \
				$(addprefix $(DIR_REDI)/, $(FUNC_REDI)) \

MAIN		= main.c

SRC			= 	$(addprefix $(DIR_SRC)/, $(FUNC)) \
				$(addprefix $(DIR_SRC)/, $(MAIN))

OBJ			= $(SRC:$(DIR_SRC)/%.c=$(DIR_OBJ)/%.o)

LIB			= -L$(DIR_LIB) -lft -lreadline

DEP			= $(DIR_OBJS) $(HEADER) #$(DIR_LIB)/libft.a

INC			= -I$(DIR_INC) -I$(DIR_LIB)

CC			= cc

CFLAGS		= #-Wall -Wextra -Werror

# **************************************************************************** #
#		Unit test variables													   #
# **************************************************************************** #

T_NAME		= utest

T_DIR		= test

T_SRC		= utest.c

T_OBJ		= $(filter-out $(T_EXCL), $(OBJ)) $(addprefix $(T_DIR)/, $(T_SRC))

T_INC		= -I$(HOME)/Criterion/include/ \
				-I$(HOME)/Criterion/ $(INC)

T_LIB		= -Wl,-rpath=$(HOME)/Criterion/build/src \
				-L$(HOME)/Criterion/build/src \
				-L$(HOME)/Criterion \
				-lcriterion \
				-L$(DIR_LIB) -lft -lreadline

T_CC		= $(CC) $(CFLAGS) $(T_INC) -g

T_EXCL		= obj/main.o

V_FLAG		= valgrind --leak-check=full --show-leak-kinds=all \
				--track-origins=yes --track-fds=yes --trace-children=yes \
				--suppressions=inc/readline.supp

# **************************************************************************** #
#																			   #
#			COMPILATION														   #
#																			   #
# **************************************************************************** #

all:				$(NAME)

$(NAME):			$(DEP) $(OBJ)
					$(MAKE) -C $(DIR_LIB)
					$(CC) $(CFLAGS) $(INC) -o $(NAME) $(OBJ) $(LIB)

$(DIR_OBJ)/%.o:		$(DIR_SRC)/%.c
					$(CC) $(CFLAGS) $(INC) -c $< -o  $@

$(DIR_OBJS):
					mkdir -p $@

$(LIB):
					make -C $(DIR_LIB)


# **************************************************************************** #
#																			   #
#			TEST AND DEBUG													   #
#																			   #
# **************************************************************************** #

debug:		$(DEP) $(OBJ)
			$(MAKE) -C $(DIR_LIB)
			@echo "Compiling with debug flag"
			$(CC) $(CFLAGS) -g $(INC) -o $(NAME) $(SRC) $(LIB)

$(T_NAME):	$(DEP) $(OBJ)
			@echo "Compiling unit test"
			$(T_CC) $(T_OBJ) -o $(T_DIR)/$(T_NAME) -L$(DIR_LIB) $(LIB) $(T_LIB)

test:		$(T_NAME)
			@echo "Running unit tests :"
			./$(T_DIR)/$(T_NAME)

valgrind:	debug
			@echo "Running with valgrind :"
			$(V_FLAG) ./$(NAME)

# **************************************************************************** #
#																			   #
#			CLEAN UP														   #
#																			   #
# **************************************************************************** #

clean:
			rm -rf $(DIR_OBJ)
			rm -rf $(T_DIR)/$(T_NAME)
			make -C $(DIR_LIB) clean

fclean:
			rm -rf $(DIR_OBJ)
			rm -rf $(T_DIR)/$(T_NAME)
			rm -rf $(NAME)
			make -C $(DIR_LIB) fclean

re:			fclean all

.PHONY:		all clean fclean re debug test compile
