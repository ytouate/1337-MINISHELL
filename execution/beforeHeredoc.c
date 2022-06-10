/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   beforeHeredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 13:35:25 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/10 10:29:31 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void	exec_first_node_before_herdoc(t_vars *vars, t_norm data)
{
	close(data.fd[0]);
	dup2(data.fd[1], STDOUT_FILENO);
	exec_node(vars, vars->command, data.contex);
}

void	exec_last_node_before_heredoc(t_vars *vars, t_norm data)
{
	close(data.fd[0]);
	close(data.fd[1]);
	dup2(data.temp_fd, STDIN_FILENO);
	exec_node(vars, vars->command, data.contex);
}

void	exec_other_node_before_heredoc(t_vars *vars, t_norm data)
{
	close(data.fd[0]);
	dup2(data.fd[1], STDOUT_FILENO);
	dup2(data.temp_fd, STDIN_FILENO);
	exec_node(vars, vars->command, data.contex);
}

void	wait_for_child(int *ids, int i)
{
	while (--i >= 0)
		waitpid(ids[i], 0, 0);
}

void	loop_through_nodes(t_vars *vars, t_norm data)
{
	int	j;
	j = 0;
	while (vars->command->herdoc == NULL)
	{
		data.contex.fd_in = STDIN_FILENO;
		data.contex.fd_out = STDOUT_FILENO;
		pipe(data.fd);
		data.id = fork();
		if (data.id == 0)
		{
			replace_symbol_by_val(vars->command->flags, vars->env_list);
			if (data.i == 0)
				exec_first_node_before_herdoc(vars, data);
			else if (data.i == data.size - 1)
				exec_last_node_before_heredoc(vars, data);
			else
				exec_other_node_before_heredoc(vars, data);
			exit(0);
		}
		data.ids[j++] = data.id;
		data.temp_fd = dup(data.fd[0]);
		close(data.fd[0]);
		close(data.fd[1]);
		data.i += 1;
		vars->command = vars->command->next_command;
	}
	close(data.temp_fd);
	wait_for_child(data.ids, j);
}

int count_commands_before_heredoc(t_command command)
{
    int counter;
    counter = 0;
    while (command.herdoc == NULL)
    {
        counter += 1;
        command = *command.next_command;
    }
    return (counter);
}

void	execCommandsBeforeHeredoc(t_vars *vars)
{
	t_norm	data;

	data.contex.fd_in = STDIN_FILENO;
	data.contex.fd_out = STDOUT_FILENO;
	data.size = count_commands_before_heredoc(*vars->command);
	data.ids = malloc(sizeof(int) * data.size);
	data.i = 0;
}