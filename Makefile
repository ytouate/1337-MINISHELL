# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/05/31 13:17:58 by ytouate           #+#    #+#              #
#    Updated: 2022/06/11 13:13:44 by ytouate          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

all :
	@cd execution && make && cd ..
	@cd parsing && make && cd ..
	@cd libft && make && make bonus && cd ..
	@gcc -Wall -Wextra -Werror ./libft/libft.a ./execution/exec.a ./parsing/pars.a main.c -o minishell -lreadline \
	-L /Users/ytouate/.brew/Cellar/readline/8.1.2/lib -I \
	/Users/ytouate/.brew/Cellar/readline/8.1.2/include

clean :
	@cd execution && make clean && cd ..
	@cd libft && make fclean && cd ..
	@cd parsing && make clean && cd ..

fclean :
	@cd execution && make fclean && cd ..
	@cd libft && make fclean && cd ..
	@cd parsing && make fclean && cd ..