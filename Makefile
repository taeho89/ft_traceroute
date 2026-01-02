NAME = ft_traceroute

CC = cc
CFLAGS = -g

SRC_DIR = srcs/
SRCS =	\
		ft_traceroute.c	\
		traceroute_output.c	\
		main_loop.c		\
		packet.c		\
		icmp.c			\
		init.c			\
		utils.c

OBJ_DIR = objs/
OBJS = $(addprefix $(OBJ_DIR), $(SRCS:.c=.o))

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)%.o : $(SRC_DIR)%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $^ -o $@

$(OBJ_DIR) :
	@mkdir -p $@

clean :
	rm -rf $(OBJ_DIR)

fclean : clean
	rm -rf $(NAME)

re : fclean all

.PHONY : all clean fclean all
