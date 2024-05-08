NAME = pipex
INCLUDE = pipex.h
LIB = libft.a
SRCS = main.c parsing.c initialization.c validation.c
OBJ = $(SRCS:%.c=%.o)
CFLAGS	=	-Wall -Wextra -Werror

all		:	$(NAME)

$(NAME)	:	$(OBJ) $(INCLUDE) $(LIB)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIB) 

$(LIB)	:
	cd libft && make && make bonus && make clean && mv libft.a ../libft.a && cd ../

%.o		:	%.c $(INCLUDE) $(LIB)
	$(CC) $(CFLAGS) -c $< -o $@


clean	:
	$(RM) $(OBJ)

fclean	:	clean
	$(RM) $(OBJ) $(NAME)

re		:	fclean all

.PHONY	:	all clean fclean re