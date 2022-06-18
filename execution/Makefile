# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/05/31 12:46:19 by ytouate           #+#    #+#              #
#    Updated: 2022/06/18 11:43:55 by ytouate          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc
CFLAGS = -Wall -Wextra -Werror
NAME = exec.a
FILES =	\
	./randomUtils.c			\
	./redirection_utils.c	\
	./cd.c					\
	./builtInUtils.c		\
	./heredoc.c				\
	./before_heredoc.c		\
	./export_utils.c		\
	./heredoc_utils.c		\
	./redirection.c			\
	./pipe_utils.c			\
	./exec_utils.c			\
	./env.c					\
	./echo.c				\
	./Unset.c				\
	./free.c				\
	./pipe.c				\
	./execSimpleCommand.c	\
	./export.c				\
	./ft_exit.c				\
	./ft_pwd.c				\
	./combine.c				\
	./error_files.c			\
	./echo_utils.c			\
	./exit_utils.c

O_FILES = $(FILES:.c=.o)

all: $(NAME)

$(NAME): $(O_FILES)
	@ar -rc  exec.a $(O_FILES) 

clean:
	@rm -f $(O_FILES)

fclean: clean
	@rm -f exec.a

re: fclean all
