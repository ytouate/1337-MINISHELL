# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/05/31 13:17:58 by ytouate           #+#    #+#              #
#    Updated: 2022/06/14 14:49:11 by ytouate          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = gcc
EXEC = ./execution/exec.a
PARSE = ./parsing/pars.a
LIBFT = ./libft/libft.a
READLINE = -lreadline -L /Users/ytouate/.brew/Cellar/readline/8.1.2/lib -I /Users/ytouate/.brew/Cellar/readline/8.1.2/include
CFLAGS = -Wall -Wextra -Werror $(LIBFT) $(EXEC) $(PARSE) main.c -o $(NAME) $(READLINE)

all :
	@cd execution && make && cd ..
	@cd parsing && make && cd ..
	@cd libft && make && make bonus && cd ..
	@gcc -Wall -Wextra -Werror ./libft/libft.a ./execution/exec.a ./parsing/pars.a main.c -o minishell -lreadline $(READLINE)

clean :
	@cd execution && make fclean && cd ..
	@cd libft && make fclean && cd ..
	@cd parsing && make fclean && cd ..

fclean : clean
	@rm -rf minishell