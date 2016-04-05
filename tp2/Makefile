NAME=	schd
SRC=	main.c		\
	create_ctx.c	\
	init_ctx.c	\
	core.c		\
	start_sched.c
OBJ=	$(SRC:.c=.o)
CC=	gcc
CFLAGS=	-g -W -Wall -m32 -pthread $(INC)
LIB=	-L/home/m1/lheureux/lib -lhardware
INC=	-I/home/m1/lheureux/include
RM=	@rm -fv

$(NAME):	$(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIB)

clean:
	$(RM) $(OBJ)

fclean:
	$(RM) $(NAME)

re: clean fclean $(NAME)
