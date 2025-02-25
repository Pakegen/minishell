/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: quenalla <quenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 03:57:10 by qacjl             #+#    #+#             */
/*   Updated: 2025/02/25 14:54:10 by quenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*append_str(char *dest, const char *src)
{
	size_t	len_dest;
	size_t	len_src;
	size_t	i;
	size_t	j;
	char	*new_str;

	len_dest = ft_strlen(dest);
	len_src = ft_strlen(src);
	new_str = malloc(len_dest + len_src + 1);
	if (new_str == NULL)
		return (NULL);
	i = 0;
	while (dest && dest[i])
	{
		new_str[i] = dest[i];
		i++;
	}
	j = 0;
	while (src[j])
	{
		new_str[i + j] = src[j++];

	}
	new_str[i + j] = '\0';
	if (dest)
		free(dest);
	return (new_str);
}

static char	*expand_var(const char *input, int *i)
{
	int		j;
	char	*var_name;
	char	*value;

	j = *i + 1;
	while (input[j] && ((input[j] >= 'A' && input[j] <= 'Z')
			|| (input[j] >= 'a' && input[j] <= 'z')
			|| (input[j] >= '0' && input[j] <= '9')
			|| (input[j] == '_')))
	{
		j++;
	}
	var_name = ft_strndup(input + *i + 1, j - *i - 1);
	value = getenv(var_name);
	if (value == NULL)
		value = "";
	free(var_name);
	*i = j;
	return (ft_strdup(value));
}

/*
char	*expand_variables(const char *input)
{
	int		i;
	char	*result;
	char	*temp;
	char	str[2];

	i = 0;
	result = ft_strdup("");
	while (input[i])
	{
		if (input[i] == '$')
		{
			temp = expand_var(input, &i);
			result = append_str(result, temp);
			free(temp);
		}
		else
		{
			str[0] = input[i];
			str[1] = '\0';
			result = append_str(result, str);
			i = i + 1;
		}
	}
	return (result);
}
*/

char	*expand_variables(const char *input)
{
	int		i;
	char	*result;
	char	*temp;
	char	ch[2];

	i = 0;
	result = ft_strdup("");
	if (result == NULL)
		return (NULL);
	while (input[i])
	{
		if (input[i] == '$')
		{
			temp = expand_var(input, &i);
			result = append_str(result, temp);
			free(temp);
		}
		else
		{
			ch[0] = input[i];
			ch[1] = '\0';
			result = append_str(result, ch);
			i = i + 1;
		}
	}
	return (result);
}
