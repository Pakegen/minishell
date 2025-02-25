/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qacjl <qacjl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 17:44:58 by axbaudri          #+#    #+#             */
/*   Updated: 2025/02/19 16:20:58 by qacjl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_export(t_shell *shell, t_prompt *prompt)
{
	if (count_words(prompt->cmd_line) == 1)
		write_env(shell->export_lines);
}
