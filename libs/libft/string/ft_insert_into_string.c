#include "../libft.h"

char *ft_insert_into_string(char **base, char *insert, char *base_delimiter)
{
	size_t base_len;
	size_t insert_len;
	size_t i;

	if (!base || !*base || !insert || !base_delimiter)
		return (NULL);

	base_len = ft_strlen(*base);
	insert_len = ft_strlen(insert);
	i = base_delimiter - *base;

	char *new = ft_calloc(base_len + insert_len + 1, sizeof(char));
	if (!new)
		return (NULL);

	ft_strlcpy(new, *base, i + 1);
	ft_strlcpy(new + i, insert, insert_len + 1);
	ft_strlcpy(new + i + insert_len, base_delimiter, base_len - i + 1);

	free(*base);
	*base = new;
	return (new);
}
