# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/19 11:47:53 by sgoremyk          #+#    #+#              #
#    Updated: 2024/05/19 11:47:54 by sgoremyk         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex

LIB = libft.a

INCLUDE = pipex.h

SRCS = main.c initialization.c validation.c free.c utils.c

OBJ = $(SRCS:%.c=%.o)

CFLAGS = -Wall -Wextra -Werror -g

all		:	$(NAME)

$(NAME)	:	$(OBJ) $(INCLUDE) $(LIB)
	$(CC) $(CFLAGS) $(OBJ) ./libft/libft.a -o $(NAME)

$(LIB)	:
	$(MAKE) -sC ./libft

%.o		:	%.c $(INCLUDE)
	$(CC) $(CFLAGS) -c $< -o $@

clean	:
	$(RM) $(OBJ)

fclean	:	clean
	$(RM) $(NAME)

re		:	fclean all

.PHONY	:	all clean fclean re
