#include "../libft.h"

char *ft_insert_into_string(char **base, char *insert, char *base_delimiter)
{
	size_t base_len;
	size_t insert_len;

	if (!base || !*base || !insert || !base_delimiter)
		return (NULL);

	int a;

	a = 0;
	while ((*base)[a] && (*base)[a] != *base_delimiter)
		a++;

	base_len = a;
	insert_len = ft_strlen(insert);

	char *new = ft_calloc(base_len + insert_len + 1, sizeof(char));
	if (!new)
		return (NULL);

	ft_strlcpy(new, *base, a + 1);
	ft_strlcpy(new + a, insert, insert_len + 1);
	ft_strlcpy(new + a + insert_len, base_delimiter, ft_strlen(base_delimiter) + 1);

	free(*base);
	*base = new;
	return (new);
}
