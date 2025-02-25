/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qacjl <qacjl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 14:44:05 by qacjl             #+#    #+#             */
/*   Updated: 2025/02/19 17:08:57 by qacjl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	display_history(void)
{
	HIST_ENTRY	**history_list;
	int			i;

	history_list = history_get_history_state()->entries;
	i = 0;
	if (history_list)
	{
		while (history_list[i])
		{
			ft_printf("%d  %s\n", i + 1, history_list[i]->line);
			i++;
		}
	}
}

void	verif_history(const char *input)
{
	if (strcmp(input, "history") == 0)
		display_history();
	else
		ft_printf("Exécution de la commande : %s\n", input);
}

static char	**build_args(char *cmd)
{
	char	**args;
	int		i;
	char	*token;

	args = malloc(sizeof(char *) * MAX_ARGS);
	if (args == NULL)
		exit(EXIT_FAILURE);
	i = 0;
	token = strtok(cmd, " \t\n");
	while (token != NULL && i < MAX_ARGS - 1)
	{
		args[i] = token;
		i = i + 1;
		token = strtok(NULL, " \t\n");
	}
	args[i] = NULL;
	return (args);
}

static void	do_redirection(int src, int dest)
{
	if (dup2(src, dest) == -1)
	{
		perror("dup2");
		exit(EXIT_FAILURE);
	}
	close(src);
}

void	execute_command(char *cmd, int in_fd, int out_fd)
{
	pid_t	pid;
	char	**args;

	args = build_args(cmd);
	pid = fork();
	if (pid == 0)
	{
		if (in_fd != STDIN_FILENO)
			do_redirection(in_fd, STDIN_FILENO);
		if (out_fd != STDOUT_FILENO)
			do_redirection(out_fd, STDOUT_FILENO);
		execvp(args[0], args);
		perror("execvp");
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	free(args);
}

static int	build_cmds(char *input, char *cmds[256])
{
	int		num_cmds;
	char	*token;

	num_cmds = 0;
	token = strtok(input, "|");
	while (token != NULL)
	{
		cmds[num_cmds] = token;
		num_cmds = num_cmds + 1;
		token = strtok(NULL, "|");
	}
	return (num_cmds);
}

void	execute_pipes(char *input)
{
	int		num_cmds;
	int		i;
	int		fd[2];
	int		in_fd;
	int		out_fd;
	char	*cmds[256];

	in_fd = STDIN_FILENO;
	num_cmds = build_cmds(input, cmds);
	i = 0;
	while (i < num_cmds)
	{
		out_fd = STDOUT_FILENO;
		if ((i + 1) < num_cmds)
		{
			if (pipe(fd) == -1)
			{
				perror("pipe");
				exit(EXIT_FAILURE);
			}
			out_fd = fd[1];
		}
		execute_command(cmds[i], in_fd, out_fd);
		if (out_fd != STDOUT_FILENO)
			close(fd[1]);
		if ((i + 1) < num_cmds)
			in_fd = fd[0];
		i = i + 1;
	}
	i = 0;
	while (i < num_cmds)
	{
		wait(NULL);
		i = i + 1;
	}
}
