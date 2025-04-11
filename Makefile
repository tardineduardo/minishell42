MAIN 		=  	src/minishell.c \

READLINE 	=	src/readline.c \

HEREDOC 	=	src/heredocs.c \

EXECUTE 	=	src/execute.c \

MEMORY 		=	src/memory.c \

TOKEN 		=	src/tokenize.c \
				src/expand_heredoc.c \
				src/expand_shared.c \
				src/expand_token.c \



ENVIRON 	=	src/environs.c \

OPERATORS 	=	src/operators.c \

BUILTIN 	=	src/built-ins/ft_env.c \
				src/built-ins/ft_pwd.c \
				src/built-ins/ft_exit.c \
				src/built-ins/ft_echo.c \
				src/built-ins/ft_cd.c \
				src/built-ins/ft_unset.c \
				src/built-ins/ft_export.c \




OBJS_SRC =	$(MAIN:.c=.o) \
			$(HEREDOC:.c=.o) \
			$(MEMORY:.c=.o) \
			$(TOKEN:.c=.o) \
			$(ENVIRON:.c=.o) \
			$(EXECUTE:.c=.o) \
			$(READLINE:.c=.o) \
			$(BUILTIN:.c=.o) \
			$(OPERATORS:.c=.o) \


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