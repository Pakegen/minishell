/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adv_tokeniser.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qacjl <qacjl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 03:00:00 by qacjl             #+#    #+#             */
/*   Updated: 2025/02/24 13:18:23 by qacjl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
static char	*append_char(char *str, char c)
{
	size_t	len;
	char	*new_str;
	size_t	i;

	if (str)
		len = ft_strlen(str);
	else
		len = 0;
	new_str = malloc(len + 2);
	if (!new_str)
		return (NULL);
	i = 0;
	while (str && str[i])
	{
		new_str[i] = str[i];
		i++;
	}
	new_str[i] = c;
	new_str[i + 1] = '\0';
	if (str)
		free(str);
	return (new_str);
}

static void	process_char(char c, t_state *state, char **curr)
{
	if (*state == STATE_DEFAULT)
	{
		switch (c)
		{
			case ' ':
			case '\t': break;
			case '\'': *state = STATE_IN_SINGLE; break;
			case '"': *state = STATE_IN_DOUBLE; break;
			case '\\': *state = STATE_ESCAPING; break;
			default: *curr = append_char(*curr, c); break;
		}
	}
	else if (*state == STATE_IN_SINGLE)
		*state = (c == '\'') ? STATE_DEFAULT : (*curr = append_char(*curr, c), STATE_IN_SINGLE);
	else if (*state == STATE_IN_DOUBLE)
		*state = (c == '"') ? STATE_DEFAULT : (c == '\\') ? STATE_ESCAPING : (*curr = append_char(*curr, c), STATE_IN_DOUBLE);
	else if (*state == STATE_ESCAPING) { *curr = append_char(*curr, c); *state = STATE_DEFAULT; }
}

char	**advanced_tokenize(const char *line)
{
	int			i;
	int			ti;
	char		**tokens;
	char		*curr;
	t_state		state;

	tokens = malloc(sizeof(char *) * (ft_strlen(line) + 1));
	if (!tokens)
		return (NULL);
	i = 0;
	ti = 0;
	curr = NULL;
	state = STATE_DEFAULT;
	while (line[i])
	{
		process_char(line[i], &state, &curr);
		if (state == STATE_DEFAULT && (line[i] == ' ' || line[i] == '\t'))
		{
			if (curr)
			{
				tokens[ti] = curr;
				ti++;
				curr = NULL;
			}
		}
		i++;
	}
	if (curr)
	{
		tokens[ti] = curr;
		ti++;
	}
	tokens[ti] = NULL;
	return (tokens);
}
*/


#include "minishell.h"

typedef enum e_state
{
	STATE_DEFAULT,
	STATE_IN_SINGLE,
	STATE_IN_DOUBLE,
	STATE_ESCAPING
}				t_state;

static char	*append_char(char *str, char c)
{
	size_t	len;
	char	*new_str;
	size_t	i;

	if (str)
		len = ft_strlen(str);
	else
		len = 0;
	new_str = malloc(len + 2);
	if (new_str == NULL)
		return (NULL);
	i = 0;
	while (str && str[i])
	{
		new_str[i] = str[i];
		i++;
	}
	new_str[i] = c;
	new_str[i + 1] = '\0';
	if (str)
		free(str);
	return (new_str);
}

static void	process_default(char c, t_state *state, char **curr)
{
	if (c == ' ' || c == '\t')
		return ;
	if (c == '\'')
	{
		*state = STATE_IN_SINGLE;
		return;
	}
	if (c == '"')
	{
		*state = STATE_IN_DOUBLE;
		return;
	}
	if (c == '\\')
	{
		*state = STATE_ESCAPING;
		return;
	}
	*curr = append_char(*curr, c);
}

static void	process_in_single(char c, t_state *state, char **curr)
{
	if (c == '\'')
		*state = STATE_DEFAULT;
	else
		*curr = append_char(*curr, c);
}

static void	process_in_double(char c, t_state *state, char **curr)
{
	if (c == '"')
		*state = STATE_DEFAULT;
	else if (c == '\\')
		*state = STATE_ESCAPING;
	else
		*curr = append_char(*curr, c);
}

static void	process_escaping(char c, t_state *state, char **curr)
{
	*curr = append_char(*curr, c);
	*state = STATE_DEFAULT;
}

static void	process_char(char c, t_state *state, char **curr)
{
	if (*state == STATE_DEFAULT)
	{
		process_default(c, state, curr);
		return;
	}
	if (*state == STATE_IN_SINGLE)
	{
		process_in_single(c, state, curr);
		return;
	}
	if (*state == STATE_IN_DOUBLE)
	{
		process_in_double(c, state, curr);
		return;
	}
	if (*state == STATE_ESCAPING)
	{
		process_escaping(c, state, curr);
		return;
	}
}

static void	tokenize_loop(const char *line, int *i, int *ti,
		char ***tokens, char **curr, t_state *state)
{
	while (line[*i])
	{
		process_char(line[*i], state, curr);
		if ((*state) == STATE_DEFAULT && (line[*i] == ' ' || line[*i] == '\t'))
		{
			if (*curr != NULL)
			{
				(*tokens)[*ti] = *curr;
				*ti = *ti + 1;
				*curr = NULL;
			}
		}
		*i = *i + 1;
	}
}

char	**advanced_tokenize(const char *line)
{
	int			i;
	int			ti;
	char		**tokens;
	char		*curr;
	t_state		state;

	tokens = malloc(sizeof(char *) * (ft_strlen(line) + 1));
	if (tokens == NULL)
		return (NULL);
	i = 0;
	ti = 0;
	curr = NULL;
	state = STATE_DEFAULT;
	tokenize_loop(line, &i, &ti, &tokens, &curr, &state);
	if (curr != NULL)
	{
		tokens[ti] = curr;
		ti = ti + 1;
	}
	tokens[ti] = NULL;
	return (tokens);
}
