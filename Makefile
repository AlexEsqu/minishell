# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mkling <mkling@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/14 14:56:12 by mkling            #+#    #+#              #
#    Updated: 2024/11/20 18:49:54 by mkling           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= minishell

SRC_DIR		= ./src

OBJ_DIR		= ./obj

INC_DIR		= ./inc

FUNC		= parse.c

MAIN		= main.c

SRC			= $(FUNC) $(MAIN)

OBJS		= $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(addprefix $(SRC_DIR)/, $(SRC)))

CC			= cc

CFLAGS		= -Wall -Wextra -Werror


######################
# Unit test variables
#######################

T_NAME		= utest

T_DIR		= ./test

T_SRC		= utest.c

T_OBJ		= utest.o

T_INC		= -I${HOME}/Criterion/include/criterion

T_LIB		= -Wl,-rpath=${HOME}/Criterion/build/src -L${HOME}/Criterion/build/src

T_FLAG		= -lcriterion

T_CC		= gcc $(T_INC) $(T_LIB) $(CFLAGS) $(T_FLAG)



#########################
#		COMPILE			#
#########################

all:		$(NAME)

$(NAME):	$(OBJS) ./inc/minishell.h
			$(CC) $(CFLAGS) -o $(NAME) $(addprefix $(SRC_DIR)/, $(SRC))

$(OBJ_DIR)/%.o:		$(SRC_DIR)/%.c
					mkdir -p $(OBJ_DIR)
					$(CC) $(CFLAGS) $(INC) -c $< -o $@

#############################
#		TEST AND DEBUG		#
#############################

debug:		$(OBJS) ./inc/minishell.h
			@echo "Compiling with debug flag"
			$(CC) $(CFLAGS) -g -o $(NAME) $(addprefix $(SRC_DIR)/, $(SRC))

test:
			@echo "Compiling test code"
			$(T_CC) $(INC) $(T_INC) -c $(T_DIR)/$(T_SRC) -o $(T_DIR)/$(T_OBJ)
			@echo "Compiling source code without main"
			$(CC) $(CFLAGS) $(INC) -c $(addprefix $(SRC_DIR)/, $(FUNC)) -o $(addprefix $(T_DIR)/, $(FUNC))
			@echo "Compiling unit test"
			${T_CC} $(addprefix $(SRC_DIR)/, $(FUNC)) -o $(addprefix $(T_DIR)/, $(FUNC)) $(T_DIR)/${T_OBJ} -o $(T_DIR)/${T_NAME}
			@echo "Running unit tests"
			$(T_DIR)/$(T_NAME)

#########################
#		CLEAN UP		#
#########################

clean:
			rm -rf $(OBJ_DIR)
			rm -rf $(T_DIR)/$(T_OBJ)
			rm -rf $(T_DIR)/$(T_NAME)

fclean:		clean
			rm -rf $(NAME)

re:			fclean all

.PHONY:		all clean fclean re debug test
