PRINT =	printf/ft_printf_dprintf.c \
		printf/ft_printf_int_chr_str.c \
		printf/ft_printf_hex_ptr_uns.c \

PUTS =	puts/ft_putchar_fd.c \
		puts/ft_puthex_int_low_fd.c \
		puts/ft_puthex_int_upp_fd.c \
		puts/ft_puthex_long_low_fd.c \
		puts/ft_putnbr_fd.c \
		puts/ft_putstr_fd.c \
		puts/ft_putuns_fd.c \

CONV =	conv/ft_atoi.c \
		conv/ft_atol.c \
		conv/ft_atohx.c \
		conv/ft_itoa.c \
		conv/ft_tolower.c \
		conv/ft_toupper.c \
		conv/ft_abs.c \

CALC = 	calc/ft_lowest.c \
		calc/ft_greatest.c \

ERROR =	error/ft_print_error_and.c \
		error/ft_perror_exit.c \
		error/ft_perror_func_exit.c	\
		error/ft_perror_extra.c \
		error/ft_null_and_stderr.c \
		fds/ft_close.c \

FDS =	fds/ft_close_four.c \
		fds/ft_close_three.c \
		fds/ft_close_two.c \
		fds/ft_close.c \
		fds/ft_count_all_lines_fd.c \
		fds/ft_count_all_lines_fn.c \
		fds/ft_count_nonempty_lines_fd.c \
		fds/ft_count_nonempty_lines_fn.c \

MEM = 	mem/ft_bzero.c \
		mem/ft_calloc.c \
		mem/ft_memcpy.c \
		mem/ft_free_and.c \

STRING = string/ft_concatenate.c \
		string/ft_concatenate_var.c \
		string/ft_split_char.c \
		string/ft_split_space.c \
		string/ft_split_count.c \
		string/ft_strchr.c \
		string/ft_strdup.c \
		string/ft_striteri.c \
		string/ft_strjoin.c \
		string/ft_strlcat.c \
		string/ft_strlcpy.c \
		string/ft_strlen.c \
		string/ft_strmapi.c \
		string/ft_strncmp.c \
		string/ft_strcmp.c \
		string/ft_strnstr.c \
		string/ft_strrchr.c \
		string/ft_strtrim.c \
		string/ft_substr.c \
		string/ft_count_char.c \
		string/ft_strtok.c \
		string/ft_strtok_r.c \
		string/ft_fn_to_str.c \
		string/ft_cat.c	\

VALID =	valid/ft_isalnum.c \
		valid/ft_isalpha.c \
		valid/ft_isascii.c \
		valid/ft_isdigit.c \
		valid/ft_isprint.c \
		valid/ft_isspace.c \
		valid/ft_is_line_empty.c \

SORT =	sort/ft_sort_int_array_bubble.c

GNL	=	gnl/get_next_line_bonus.c

LISTS =	lists/slist/ft_lstadd_back.c \
		lists/slist/ft_lstadd_front.c \
		lists/slist/ft_lstclear.c \
		lists/slist/ft_lstclear_err_exit.c \
		lists/slist/ft_lstclear_null.c \
		lists/slist/ft_lstdelone.c \
		lists/slist/ft_lstiter.c \
		lists/slist/ft_lstlast.c \
		lists/slist/ft_lstmap.c \
		lists/slist/ft_lstnew.c \
		lists/slist/ft_lstsize.c \


OBJS =	$(PRINT:.c=.o) \
		$(CONV:.c=.o) \
		$(CALC:.c=.o) \
		$(ERROR:.c=.o) \
		$(FDS:.c=.o) \
		$(MEM:.c=.o) \
		$(LISTS:.c=.o) \
		$(PUTS:.c=.o) \
		$(STRING:.c=.o) \
		$(VALID:.c=.o) \
		$(GNL:.c=.o) \
		$(SORT:.c=.o) \

CC = cc

RM = rm -f

CFLAGS = -Wall -Wextra -Werror

NAME = libft.a

all: $(NAME)

$(NAME): $(OBJS)
	@ar rc $(NAME) $(OBJS)
	@ranlib $(NAME)
	@$(MAKE) clean

%.o: %.c libft.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
