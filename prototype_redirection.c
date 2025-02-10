/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prototype_redirection.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qacjl <qacjl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 01:25:41 by qacjl             #+#    #+#             */
/*   Updated: 2025/02/10 01:43:25 by qacjl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_redirection(const char *file, int io_flag)
{
	int	fd;

	if (io_flag == O_RDONLY)
	{
		fd = open(file, O_RDONLY);
		if (fd < 0)
		{
			perror("Erreur lors de l'ouverture du fichier");
			return (-1);
		}
		dup2(fd, STDIN_FILENO);
	}
	else
	{
		fd = open(file, io_flag, 0644);
		if (fd < 0)
		{
			perror("Erreur lors de l'ouverture du fichier");
			return (-1);
		}
		dup2(fd, STDOUT_FILENO);
	}
	close(fd);
	return (0);
}

void	handle_pipe(char *cmd1[], char *cmd2[])
{
	int pipe_fd[2];
	pid_t pid1, pid2;

	if (pipe(pipe_fd) == -1)
	{
		perror("Erreur lors de la création du pipe");
		exit(EXIT_FAILURE);
	}
	if ((pid1 = fork()) == 0)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
		execve(cmd1[0], cmd1, NULL);
		perror("Erreur lors de l'exécution de la commande");
		exit(EXIT_FAILURE);
	}
	if ((pid2 = fork()) == 0)
	{
		close(pipe_fd[1]);
		dup2(pipe_fd[0], STDIN_FILENO);
		close(pipe_fd[0]);
		execve(cmd2[0], cmd2, NULL);
		perror("Erreur lors de l'exécution de la commande");
		exit(EXIT_FAILURE);
	}
	close(pipe_fd[0]);
	close(pipe_fd[1]);

	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
}

int main2(void)
		if (strstr(input, ">"))
		{
			char *cmd = strtok(input, "> ");
			char *file = strtok(NULL, " ");
			if (cmd && file)
			{
				if (handle_redirection(file, O_WRONLY | O_CREAT | O_TRUNC) == 0)
				{
					char *args[] = {cmd, NULL};
					execve(cmd, args, NULL);
				}
			}
		}
		else if (strstr(input, "|"))
		{
			char *cmd1 = strtok(input, "|");
			char *cmd2 = strtok(NULL, "");
			if (cmd1 && cmd2)
			{
				char *args1[] = {cmd1, NULL};
				char *args2[] = {cmd2, NULL};
				handle_pipe(args1, args2);
			}
		}


void	display_history()
{
	HIST_ENTRY	**history_list;
	int	i;

	**history_list = history_list();
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
/*
int main(void)
{
    char *input;

    // Charger l'historique depuis un fichier
    read_history("history.txt");
    using_history();

    while (1)
    {
        input = readline("minishell> ");

        if (!input)
        {
            printf("exit\n");
            break;
        }

        // Ajoute la commande à l'historique si elle n'est pas vide
        if (*input)
        {
            add_history(input);
        }

        // Vérifie si la commande est "history"
        if (strcmp(input, "history") == 0)
        {
            display_history();
        }
        else
        {
            printf("Exécution de la commande : %s\n", input);
        }

        free(input);
    }

    // Sauvegarder l'historique dans un fichier avant de quitter
    write_history("history.txt");

    return 0;
}
a incorporer
*/
