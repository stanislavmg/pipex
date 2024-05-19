path = pipex

INCLUDE = pipex.h

SRCS = main.c initialization.c validation.c

OBJ = $(SRCS:%.c=%.o)

CFLAGS = -Wall -Wextra -Werror

all		:	$(path)

$(path)	:	$(OBJ) $(INCLUDE)
	$(MAKE) -sC ./libft
	$(CC) $(CFLAGS) $(OBJ) ./libft/libft.a -o $(path)

%.o		:	%.c $(INCLUDE)
	$(CC) $(CFLAGS) -c $< -o $@

clean	:
	$(RM) $(OBJ)

fclean	:	clean
	$(RM) $(OBJ) $(path)

re		:	fclean all

.PHONY	:	all clean fclean re