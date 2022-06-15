# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/05/31 13:17:58 by ytouate           #+#    #+#              #
#    Updated: 2022/06/15 12:07:24 by ytouate          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = gcc
CFLAGS = -Wall -Wextra -Werror -lreadline -L /Users/ilefhail/homebrew/Cellar/readline/8.1.2/lib -I /Users/ilefhail/homebrew/Cellar/readline/8.1.2/include


all : $(NAME)

$(NAME) :
	@cd execution && make && cd ..
	@cd parsing && make && cd ..
	@cd libft && make && make bonus && cd ..
	@$(CC)  $(CFLAGS)  ./libft/libft.a ./execution/exec.a  ./parsing/pars.a main.c -o $(NAME) 

clean :
	@cd execution && make fclean && cd ..
	@cd libft && make fclean && cd ..
	@cd parsing && make fclean && cd ..

fclean : clean
	@rm -rf minishell

re : fcelan all