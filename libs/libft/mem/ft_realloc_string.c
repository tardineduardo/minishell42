#include "../libft.h"


char	*ft_realloc_string(char **string, size_t newlen)
{
	char	*new;

	if (!string || !(*string))
		return (NULL);

	new = ft_calloc(newlen, 1);
	if (!new)
		return (NULL);

	ft_strlcpy(new, *string, newlen + 1);
	ft_free_and_null((void *)&(*string));
	*string = new;
	return (new);
}
