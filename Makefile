NAME = ft_traceroute

CC = cc
CFLAGS = -g

SRC_DIR = srcs/
SRCS =	\
		ft_traceroute.c	\
		traceroute_output.c	\
		utils.c

OBJ_DIR = objs/
OBJS = $(addprefix $(OBJ_DIR), $(SRCS:.c=.o))

LIBFT = libft

all : $(NAME)

$(NAME) : $(OBJS)
	@make -s --no-print-directory -C $(LIBFT)
	$(CC) $(CFLAGS) $^ -Llibft -lft -o $@

$(OBJ_DIR)%.o : $(SRC_DIR)%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $^ -o $@

$(OBJ_DIR) :
	@mkdir -p $@

clean :
	@make -s --no-print-directory -C $(LIBFT) clean
	rm -rf $(OBJ_DIR)

fclean : clean
	@make -s --no-print-directory -C $(LIBFT) fclean
	rm -rf $(NAME)

re : fclean all

.PHONY : all clean fclean all
