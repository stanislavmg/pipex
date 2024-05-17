NAME = pipex
INCLUDE = pipex.h
SRCS = main.c initialization.c validation.c
OBJ = $(SRCS:%.c=%.o)
CFLAGS	=	-Wall -Wextra -Werror

all		:	$(NAME)

$(NAME)	:	$(OBJ) $(INCLUDE)
	$(MAKE) -sC ./libft
	$(CC) $(CFLAGS) $(OBJ) ./libft/libft.a -o $(NAME)

%.o		:	%.c $(INCLUDE)
	$(CC) $(CFLAGS) -c $< -o $@


clean	:
	$(RM) $(OBJ)

fclean	:	clean
	$(RM) $(OBJ) $(NAME)

re		:	fclean all

.PHONY	:	all clean fclean re