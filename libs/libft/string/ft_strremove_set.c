#include "../libft.h"



char *ft_strremove_set(char *str, char *set)
{
	size_t a;
	size_t b;
	char *new;
	char *temp;
	
	if (!str || !set)
		return (NULL);
	new = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	if (!new)
		return (NULL);
	a = 0;
	b = 0;
	while (str[a])
	{
		if (!ft_strchr(set, str[a]))
		{
			new[b] = str[a];
			b++;
		}
		a++;
	}
	temp = new;
	new = ft_strdup(new);
	if (!new)
		return (free(temp), NULL);
	free(temp);
	return (new);
}

