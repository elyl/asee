NAME=	schd
SRC=	main.c		\
	shell.c		\
	commands.c 	\
	create_ctx.c	\
	init_ctx.c	\
	start_sched.c	\
	disk_ctx.c	\
	drive.c		\
	hw.c
OBJ=	$(SRC:.c=.o)
CC=	gcc
CFLAGS=	-g -W -Wall -m32 $(INCDIR)
RM=	@rm -fv
INCDIR=		-I/home/m1/lheureux/include
LIB=		-L/home/m1/lheureux/lib -ldrive -lhardware

$(NAME):	$(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(INCDIR) $(LIB)

clean:
	$(RM) $(OBJ)

fclean:
	$(RM) $(NAME)

re: clean fclean $(NAME)
