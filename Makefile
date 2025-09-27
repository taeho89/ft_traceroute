NAME = ft_traceroute

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
	@make -C $(LIBFT)
	$(CC) $(FLAGS) $^ -Lsrcs/libft -lft -o $@

$(OBJ_DIR)%.o : $(SRC_DIR)%.c | $(OBJ_DIR)
	$(CC) $(FLAGS) -c $^ -o $@

$(OBJ_DIR) :
	mkdir -p $@

clean :
	@make -C $(LIBFT) clean
	rm -rf $(OBJ_DIR)

fclean : clean
	@make -C $(LIBFT) fclean
	rm -rf $(NAME)

re : fclean all

.PHONY : all clean fclean all
