/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: axbaudri <axbaudri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 12:43:52 by axbaudri          #+#    #+#             */
/*   Updated: 2025/02/19 15:49:04 by axbaudri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	display_echo(t_prompt *prompt)
{
	if (prompt->strs[1] && ft_strcmp(prompt->strs[1], "-n") == 0)
	{
		prompt->echo = exec_echo(prompt->cmd_line, prompt->strs);
		ft_printf("%s", prompt->echo);
		free(prompt->echo);
	}
}

void	exec_exit(void)
{
	write(2, "exit\n", 5);
	exit(1);
}

void	execute_builtin(t_shell *shell, t_prompt *prompt)
{
	if (!ft_strlen(prompt->cmd_line) || !count_strings(prompt->strs))
		ft_printf("");
	else if (ft_strcmp(prompt->strs[0], "echo") == 0)
		display_echo(prompt);
	else if (ft_strcmp(prompt->strs[0], "export") == 0)
		exec_export(shell, prompt);
	else if (ft_strcmp(prompt->strs[0], "env") == 0
		&& count_words(prompt->cmd_line) == 1)
		write_env(shell->env_lines);
	else if (ft_strcmp(prompt->strs[0], "cd") == 0
		&& count_strings(prompt->strs) == 2)
		exec_cd(shell, prompt);
	else if (ft_strcmp(prompt->strs[0], "pwd") == 0
		&& count_words(prompt->cmd_line) == 1)
		ft_printf("%s\n", shell->pwd);
	else if (ft_strcmp(prompt->strs[0], "unset") == 0
		&& count_words(prompt->cmd_line) > 1)
		exec_unset(shell, prompt);
	else if (ft_strcmp(prompt->strs[0], "exit") == 0
		&& count_words(prompt->cmd_line) == 1)
		exec_exit();
	else if (!existing_command(shell->splitted_path, prompt->strs[0])
		&& ft_strcmp(prompt->strs[0], "history") != 0)
		ft_printf("command not found: %s\n", prompt->strs[0]);
}

int	main(int argc, char **argv, char **env)
{
	char		*line;
	t_pipeline	*pipeline;

	(void)argc;
	(void)argv;
	setup_signal();
	while (1)
	{
		line = readline("minishell> ");
		if (!line)
		{
			write(1, "exit\n", 5);
			break ;
		}
		if (line[0] != '\0')
			add_history(line);
		pipeline = parse_input(line);
		if (pipeline)
		{
			execute_pipeline(pipeline, env);
			free_pipeline(pipeline);
		}
		free(line);
	}
	return (0);
}

/*int	main(int argc, char **argv, char **envp)
{
	t_shell		*shell;
	t_prompt	*prompt;
	const char	*buffer;

	setup_signal();
	(void)argc;
	(void)argv;
	shell = init_shell(envp);
	while (1)
	{
		buffer = readline("\033[0;32mminishell> \033[0m");
		if (!buffer)
		{
			write(2, "exit\n", 5);
			break ;
		}
		add_history(buffer);
		verif_history(buffer);
		prompt = init_prompt(buffer);
		execute_builtin(shell, prompt);
		free_prompt(prompt);
	}
	free_terminal(shell);
	return (0);
}*/

/*int	main(int argc, char **argv, char **envp)
{
	t_shell		*shell;
	t_list		*temp;

	setup_signal();
	(void)argc;
	(void)argv;
	shell = init_shell(envp);
	temp = shell->vars;
	while (temp)
	{
		ft_printf("temp->content = %s\n", temp->content);
		temp = temp->next;
	}
	free_terminal(shell);
	return (0);
}*/
