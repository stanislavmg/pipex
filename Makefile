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

LIB = libft/libft.a

INCLUDE = pipex.h

SRCS = main.c initialization.c utils.c

OBJ = $(SRCS:%.c=%.o)

CFLAGS = -Wall -Wextra -Werror

all		:	$(NAME)

$(NAME)	:	$(OBJ) $(INCLUDE) $(LIB)
	$(CC) $(CFLAGS) $(OBJ) $(LIB) -o $(NAME)

$(LIB)	:
	$(MAKE) -C libft

%.o		:	%.c $(INCLUDE)
	$(CC) $(CFLAGS) -c $< -o $@

clean	:
	$(MAKE) -C libft clean
	$(RM) $(OBJ)

fclean	:	clean
	$(MAKE) -C libft fclean
	$(RM) $(NAME)

re		:	fclean all

.PHONY	:	all clean fclean re
