# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/05/19 13:43:21 by ytouate           #+#    #+#              #
#    Updated: 2022/05/19 14:08:22 by ytouate          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pars.a
CC = gcc
CFLAGS = -Wall -Wextra -Werror
SRCS = minishellparse.c parse_utils.c lexer.c lexer_utils.c

OBJS = $(SRCS:.c=.o)

all : $(NAME)

$(NAME) : $(OBJS)
	@ar -rc pars.a $(OBJS)

clean :
	@rm -rf *.o

fclean : clean
	@rm -rf pars.a

re : fclean all
