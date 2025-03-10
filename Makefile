MAIN =  src/minishell.c \
		src/ft_capture_line.c \
		src/ft_run_command.c \
		src/ms_env.c \
		src/ms_env_lst_utils.c \
		built-ins/ft_env.c \
		built-ins/ft_pwd.c \
		built-ins/ft_exit.c \
		built-ins/ft_echo.c \
		built-ins/ft_cd.c \
		built-ins/ft_export.c \

HEREDOC =	src/heredocs.c \

MEMORY =	src/memory.c \

OBJS_SRC =	$(MAIN:.c=.o) $(HEREDOC:.c=.o) $(MEMORY:.c=.o)

CC = cc
RM = rm -f
CFLAGS = -Wall -Wextra -Werror -g

NAME = minishell
LIBFT_PATH = ./libs/libft
LIBFT = $(LIBFT_PATH)/libft.a

all: $(NAME)

$(NAME): $(OBJS_SRC) $(LIBFT)
		$(CC) $(CFLAGS) $(OBJS_SRC) $(LIBFT) -o $(NAME) -lreadline

$(LIBFT): $(LIBFT_OBJS)
		$(MAKE) -C $(LIBFT_PATH) all

v:	all
		valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes --trace-children-skip='/bin/,/sbin/' --keep-debuginfo=yes \
		--suppressions=readline.supp --track-fds=yes ./$(NAME)

src/%.o: src/%.c
		$(CC) $(CFLAGS) -c $< -o $@

clean:
		$(RM) $(OBJS_SRC)
		$(MAKE) -C $(LIBFT_PATH) clean

fclean: clean
		$(RM) $(NAME)
		$(MAKE) -C $(LIBFT_PATH) fclean

re: fclean all

.PHONY: all clean fclean re