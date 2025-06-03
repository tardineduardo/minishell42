MAIN 		=  	src/minishell.c \

READLINE 	=	src/readline/readline.c \
				src/readline/readline_memory.c \
				src/readline/readline_quotes.c \

HEREDOC 	=	src/heredoc/heredocs.c \
				src/heredoc/heredocs_file.c \
				src/heredoc/heredocs_memory.c \

MEMORY 		=	src/memory.c \

TOKEN 		=	src/tokenize/tokenize_helpers.c \
				src/tokenize/tokenize_heredoc.c \
				src/tokenize/tokenize_indexing.c \
				src/tokenize/tokenize_limits.c \
				src/tokenize/tokenize_memory.c \
				src/tokenize/tokenize_operators.c \
				src/tokenize/tokenize_syntax_cases.c \
				src/tokenize/tokenize_syntax.c \
				src/tokenize/tokenize.c \

EXPAND		=	src/expand/expand_findvar.c \
				src/expand/expand_midreset.c \
				src/expand/expand_ops.c \
				src/expand/expand_reallocs.c \
				src/expand/expand_steps_1.c \
				src/expand/expand_steps_2.c \
				src/expand/expand_valids.c \
				src/expand/expand.c \

ENVIRON 	=	src/environs/environs_errors.c \
				src/environs/environs_memory.c \
				src/environs/environs.c \

PARSER		=	src/parsing/parsing_command.c \
				src/parsing/parsing_errors.c \
				src/parsing/parsing_memory.c \
				src/parsing/parsing_valids.c \
				src/parsing/parsing.c \

AST 		=	src/ast/init_ast.c \
				src/ast/ast_parse.c \
				src/ast/ast_parse_utils.c \
				src/ast/ast_memory.c \
				src/ast/ast_memory_utils.c \
				src/ast/ast_utils.c \

EXECUTE		= 	src/execution/execution.c \
				src/execution/single_cmd.c \
				src/execution/pipe_cmd.c \
				src/execution/utils.c \
				src/execution/block_treatment.c \
				src/execution/built_in.c \
				src/execution/ext_cmd.c \
				src/execution/redir_control.c \
				src/execution/redir_control_utils.c \
				src/execution/redir_control_utils_2.c \
				src/execution/exit_code.c \

SIGNALS		= 	src/signals/signals.c \
				src/signals/signals_utils.c \

BUILTIN 	=	src/built-ins/ft_env.c \
				src/built-ins/ft_pwd.c \
				src/built-ins/ft_exit.c \
				src/built-ins/ft_echo.c \
				src/built-ins/ft_cd.c \
				src/built-ins/ft_unset.c \
				src/built-ins/ft_export.c \
				src/built-ins/utils.c \

ERROR		=	src/error/error.c \

OBJS_SRC =	$(MAIN:.c=.o) \
			$(HEREDOC:.c=.o) \
			$(MEMORY:.c=.o) \
			$(TOKEN:.c=.o) \
			$(ENVIRON:.c=.o) \
			$(READLINE:.c=.o) \
			$(EXPAND:.c=.o) \
			$(BUILTIN:.c=.o) \
			$(PARSER:.c=.o) \
			$(AST:.c=.o) \
			$(EXECUTE:.c=.o) \
			$(SIGNALS:.c=.o) \
			$(ERROR:.c=.o) \

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

#strace -f -e trace=execve -e verbose=all ./minishell
