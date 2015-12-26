NAME=	schd
SRC=	main.c	\
	shell.c	\
	commands.c
OBJ=	$(SRC:.c=.o)
CC=	gcc
CFLAGS=	-g -W -Wall
RM=	@rm -fv

$(NAME):	$(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

clean:
	$(RM) $(OBJ)

fclean:
	$(RM) $(NAME)

re: clean fclean $(NAME)
