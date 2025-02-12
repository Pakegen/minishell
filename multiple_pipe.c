/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: quenalla <quenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 14:30:07 by quenalla          #+#    #+#             */
/*   Updated: 2025/02/12 15:55:26 by quenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strtok(char *str, const char *delims)
{
	int	i;

	i = 0;
	if (!str)
		return (NULL);
	while (str[i])
	{
		
	}
}

void	execute_command(char *cmd, int in_fd, int out_fd)
{
	pid_t	pid;
	char	*args[4];

	pid = fork();
	if (pid == 0)
	{
		if (in_fd != 0)
		{
			dup2(in_fd, STDIN_FILENO);
			close(in_fd);
		}
		if (out_fd != 1)
		{
			dup2(out_fd, STDOUT_FILENO);
			close(out_fd);
		}
		args[0] = "/bin/sh";
		args[1] = "-c";
		args[2] = cmd;
		args[3] = NULL;
		execve(args[0], args, NULL);
		perror("execve");
		exit(EXIT_FAILURE);
	}
}

void	execute_pipes(char *input)
{
	int		num_cmds;
	int		i;
	int		fd[2];
	int		in_fd;
	int		out_fd;
	char	*cmds[256];
	char	*token;

	num_cmds = 0;
	token = ft_strtok(input, "|");
	while (token != NULL)
	{
		cmds[num_cmds] = token;
		num_cmds++;
		token = ft_strtok(NULL, "|");
	}
	i = 0;
	in_fd = 0;
	while (i < num_cmds)
	{
		out_fd = 1;
		if (i + 1 < num_cmds)
		{
			pipe(fd);
			out_fd = fd[1];
		}
		execute_command(cmds[i], in_fd, out_fd);
		if (i + 1 < num_cmds)
		{
			close(fd[1]);
			in_fd = fd[0];
		}
		i++;
	}
	i = 0;
	while (i < num_cmds)
	{
		wait(NULL);
		i++;
	}
}
