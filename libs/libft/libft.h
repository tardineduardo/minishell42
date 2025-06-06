/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 17:02:36 by eduribei          #+#    #+#             */
/*   Updated: 2025/05/30 22:57:01 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdlib.h>
# include <limits.h>
# include <unistd.h>
# include <stdint.h>
# include <stddef.h>
# include <stdbool.h>
# include <stdarg.h>
# include <string.h>
# include <stdio.h>
# include <errno.h>
# include <fcntl.h>
# include <sys/types.h>

# ifndef GNLBUFF
#  define GNLBUFF 1
# endif

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

# define RESET       "\001\033[0m\002"

# define RED         "\001\033[31m\002"
# define GREEN       "\001\033[32m\002"
# define YELLOW      "\001\033[33m\002"
# define BLUE        "\001\033[34m\002"
# define MAGENTA     "\001\033[35m\002"
# define CYAN        "\001\033[36m\002"
# define WHITE       "\001\033[37m\002"
# define GREY        "\001\033[90m\002" 
# define BRIGHT_RED     "\001\033[91m\002"
# define BRIGHT_GREEN   "\001\033[92m\002"
# define BRIGHT_YELLOW  "\001\033[93m\002"
# define BRIGHT_BLUE    "\001\033[94m\002"
# define BRIGHT_MAGENTA "\001\033[95m\002"
# define BRIGHT_CYAN    "\001\033[96m\002"
# define BRIGHT_WHITE   "\001\033[97m\002"

//TYPEDEFS
typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

typedef struct s_dlist
{
	void			*content;
	struct s_dlist	*next;
	struct s_dlist	*prev;
}	t_dlist;

typedef struct s_dll
{
	void			*content;
	struct s_dll	*next;
	struct s_dll	*prev;
}	t_dll;

typedef struct s_btree
{
	struct s_btree	*left;
	struct s_btree	*right;
	void			*data;
}	t_btree;

typedef long long	t_llong;

// conversions + comparisons
int		ft_atoi(const char *nptr);
long	ft_atol(const char *nptr);
t_llong	ft_atohx(char *s);
char	*ft_itoa(int n);
int		ft_tolower(int c);
int		ft_toupper(int c);
int		ft_abs(int a);

//calc
int		lowest(int a, int b);
int		greatest(int a, int b);

// errors
void	ft_print_error_and_exit(char *message,
			int exit_number) __attribute__((noreturn));
void	ft_error_exit(char *message, int exit_number) __attribute__((noreturn));
bool	ft_print_error_and_false(char *message);
bool	ft_print_error_and_true(char *message);
void	*ft_print_error_and_null(char *message);
void	ft_error_free_and_exit(void *p, char *message, int exit_number);
void	ft_perror_extra(char *extra_argument, char *normal_perror);
void	ft_perror_exit(char *message,
			int exit_number) __attribute__((noreturn));
void	ft_perror_func_exit(char *message, void (*del)(void),
			int exit_number) __attribute__((noreturn));
void	*ft_null_and_stderr(char *message);

// memory
void	ft_bzero(void *s, size_t n);
void	*ft_calloc(size_t nmemb, size_t size);
void	*ft_memchr(const void *s, int c, size_t n);
int		ft_memcmp(const void *s1, const void *s2, size_t n);
void	*ft_memcpy(void *dest, const void *src, size_t n);
void	*ft_memmove(void *dest, const void *src, size_t n);
void	*ft_memset(void *s, int c, size_t n);
void	ft_free_str_array(char **array_of_chars);
void	ft_free(int total, ...);
bool	ft_free_and_false(void **ptr);
bool	ft_free_and_true(void **ptr);
void	*ft_free_and_null(void **ptr);
void	*ft_free_and_null_str_array(char ***array_of_strings);
void	*ft_free_and_exit(void **ptr, char *msg, int errnum);
char	*ft_realloc_string(char **string, size_t newlen);

// validations
int		ft_isalnum(int c);
int		ft_isalpha(int c);
int		ft_isascii(int c);
int		ft_isdigit(int c);
int		ft_isprint(int c);
int		ft_isspace(int c);
int		ft_isquote(int c);
bool	ft_is_line_empty(char *s);
bool	ft_is_single_quote(char *c);
bool	ft_is_double_quote(char *c);

// strings
char	**ft_split_char(char const *s, char c);
char	**ft_split_space(char *s);
char	*ft_strchr(const char *str, int c);
char	*ft_strdup(const char *s);
void	ft_striteri(char *s, void (*f)(unsigned int, char*));
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_split_count(char const *s, char c);
size_t	ft_strlcat(char *dst, const char *src, size_t size);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
size_t	ft_strlen(const char *s);
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strnstr(const char *big, const char *little, size_t len);
char	*ft_strrchr(const char *str, int c);
char	*ft_strtrim(char const *s1, char const *set);
char	*ft_strtrim_overwrite(char **s1, char *set);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_concatenate_var(int num, ...);
char	*ft_concatenate(char *s1, char *s2, char *s3);
size_t	ft_count_char(char *s, char c);
char	*ft_strtok(char *string, char *set);
char	*ft_strtok_r(char *input_str, char *delimiters, char **saveptr);
char	*ft_fn_to_str(char *filename);
void	ft_cat(const char *filename);
void	ft_free_split(char **results, size_t r_index); //?
char	*ft_strremove_set(char *str, char *set);

// printing
int		ft_putchar_fd(char c, int fd);
int		ft_putstr_fd(char *s, int fd);
int		ft_putnbr_fd(int n, int fd);
int		ft_puthex_upp_int(unsigned int n, int fd);
int		ft_puthex_low_int_fd(unsigned int n, int fd);
int		ft_putuns_fd(unsigned int n, int fd);
int		ft_puthex_lo_long_fd(t_llong n, int fd);

// files
void	ft_close(int total, ...);
void	ft_close_two(int fd1, int fd2);
void	ft_close_three(int fd1, int fd2, int fd3);
void	ft_close_four(int fd1, int fd2, int fd3, int fd4);
int		ft_count_all_lines_fd(int fd);
int		ft_count_all_lines_fp(char *filename);
int		ft_count_nonempty_lines_fd(int fd);
int		ft_count_nonempty_lines_fp(char *filename);

// printf
int		ft_printf(const char *input, ...);
int		ft_dprintf(int fd, const char *input, ...);
int		ft_count_and_put_chr(va_list args, int fd);
int		ft_count_and_put_str(va_list args, int fd);
int		ft_count_and_put_int(va_list args, int fd);
int		ft_count_and_put_ptr(va_list args, int fd);
int		ft_count_and_put_uns(va_list args, int fd);
int		ft_count_and_put_hex_l(va_list args, int fd);
int		ft_count_and_put_hex_u(va_list args, int fd);

// sort
void	ft_sort_int_array_bubble(int *array, size_t len);

// get next line
char	*get_next_line(int fd);

//lists - slist
void	ft_lstclear_err_exit(t_list **lst, void (*del)(void*), char *e, int n);
void	*ft_lstclear_null(t_list **lst, void (*del)(void*));
void	ft_lstadd_front(t_list **lst, t_list *new);
void	ft_lstadd_back(t_list **lst, t_list *new);
void	ft_lstclear(t_list **lst, void (*del)(void*));
void	ft_lstclear2(t_list **lst, void (*del)(void**));
void	ft_lst_destroy_node(t_list **head, t_list *node, void (*del)(void*));
void	ft_lst_unlink_node(t_list **head, t_list *node);
void	ft_lstdelone(t_list *lst, void (*del)(void*));
void	ft_lstiter(t_list *lst, void (*f)(void *));
t_list	*ft_lstlast(t_list *lst);
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));
t_list	*ft_lstnew(void *content);
int		ft_lstsize(t_list *lst);
int		ft_clstsize(t_list **tail);
void	ft_debug_print_list(t_list **head, char *type, size_t offset);
t_list	*ft_lstcopy(t_list *source);

//lists - dlist
void	ft_dlstadd_front(t_dlist **lst, t_dlist *new);
void	ft_dlstadd_back(t_dlist **lst, t_dlist *new);
void	ft_dlstclear(t_dlist **lst, void (*del)(void*));
t_dlist	*ft_dlst_destroy_node(t_dlist **head, t_dlist *node,
			void (*del)(void*));
t_dlist	*ft_dlst_quick_destroy_node(t_dlist **head, t_dlist *node,
			void (*del)(void*));
t_dlist	*ft_dlst_unlink_node(t_dlist **head, t_dlist *node);
t_dlist	*ft_dlst_quick_unlink_node(t_dlist **head, t_dlist *node);
void	ft_dlstdelone(t_dlist *lst, void (*del)(void*));
void	ft_dlstiter(t_dlist *lst, void (*f)(void *));
t_dlist	*ft_dlstlast(t_dlist *lst);
t_dlist	*ft_dlstmap(t_dlist *lst, void *(*f)(void *), void (*del)(void *));
t_dlist	*ft_dlstnew(void *content);
int		ft_dlstsize(t_dlist *lst);
void	ft_dlstinsert_between(t_dlist **lst, t_dlist *insert,
			t_dlist *start, t_dlist *end);

//lists - dclist
t_dll	*ft_dclstnew(void *content);
t_dll	*ft_dclst_find_lo_int(t_dll **tail, int offset);
t_dll	*ft_dclst_find_hi_int(t_dll **tail, int offset);
t_dll	*ft_dclst_find_value(t_dll **tail, int value, int offset);
t_dll	*ft_dclst_find_node(t_dll **tail, t_dll *node);
bool	ft_dclst_clock_sortd(t_dll **tail, char mode, int offset);
bool	ft_dclst_circ_sortd(t_dll **tail, char mode, int offset);
void	ft_dclstadd_back(t_dll **tail, t_dll *new);
void	ft_dclstclear_simple(t_dll **lst);
int		ft_dclstsize(t_dll **tail);
int		ft_dclst_dist_head_bidi(t_dll **tail, t_dll *node);
int		ft_dclst_dist_head_bidi_len(t_dll **tail, t_dll *nd, int list_len);

//debug
void	ft_debug_print_array_of_strings_line(char **array, int fd);

#endif
