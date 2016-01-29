NAME=	schd
SRC=	main.c		\
	shell.c		\
	commands.c 	\
	create_ctx.c	\
	init_ctx.c	\
	start_sched.c	\
	hw.c
OBJ=	$(SRC:.c=.o)
CC=	gcc
CFLAGS=	-g -W -Wall -m32
RM=	@rm -fv

$(NAME):	$(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

clean:
	$(RM) $(OBJ)

fclean:
	$(RM) $(NAME)

re: clean fclean $(NAME)
