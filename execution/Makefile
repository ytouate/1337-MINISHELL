# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/05/31 12:46:19 by ytouate           #+#    #+#              #
#    Updated: 2022/06/01 17:23:02 by ytouate          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc
CFLAGS = -Wall -Wextra -Werror  
NAME = exec
FILES =	\
	builtIn.c			\
	cd.c				\
	builtInUtils.c		\
	redirection.c		\
	env.c				\
	echo.c				\
	randomUtils.c		\
	Unset.c				\
	combine.c			\
	free.c				\
	pipe.c				\
	execSimpleCommand.c	\
	export.c			\
	get_next_line/get_next_line.c	\
	get_next_line/get_next_line_utils.c

O_FILES = $(FILES:.c=.o)

all: $(NAME)

$(NAME): $(O_FILES)
	@ar -rc  exec.a $(O_FILES)

clean:
	@rm -f $(O_FILES) exec.a

fclean: clean
	@rm -f exec $(NAME)

re: fclean all