/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 13:35:25 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/06 14:34:25 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"


void	exec_first_node(t_vars *vars, t_norm data)
{
	close(data.fd[0]);
	dup2(data.fd[1], STDOUT_FILENO);
	exec_node(vars, vars->command, data.contex);
}

void	exec_last_node(t_vars *vars, t_norm data)
{
	close(data.fd[0]);
	close(data.fd[1]);
	dup2(data.temp_fd, STDIN_FILENO);
	exec_node(vars, vars->command, data.contex);
}

void	exec_other_node(t_vars *vars, t_norm data)
{
	close(data.fd[0]);
	dup2(data.fd[1], STDOUT_FILENO);
	dup2(data.temp_fd, STDIN_FILENO);
	exec_node(vars, vars->command, data.contex);
}

void	wait_for_child(int *ids, int i, int temp_fd)
{
	close(temp_fd);
	while (--i >= 0)
	{
		waitpid(ids[i], 0, 0);
	}
}

int flag = 0;
void handler(int sig)
{
	if (sig == SIGUSR1)
		flag = 1;
}

void	loop_through_nodes(t_vars *vars, t_norm data)
{
	while (vars->command)
	{
		data.contex.fd_in = STDIN_FILENO;
		data.contex.fd_out = STDOUT_FILENO;
		pipe(data.fd);
		data.id = fork();
		if (data.id == 0)
		{
			replace_symbol_by_val(vars->command->flags, vars->env_list);
			if (data.i == 0)
				exec_first_node(vars, data);
			else if (data.i == data.size - 1)
				exec_last_node(vars, data);
			else
				exec_other_node(vars, data);
			exit(0);
		}
		if (flag == 1)
		{
			waitpid(data.id, 0, 0);
			flag = 0;
		}
		else
			data.ids[data.i] = data.id;
		data.temp_fd = dup(data.fd[0]);
		close(data.fd[0]);
		close(data.fd[1]);
		vars->command = vars->command->next_command;
		data.i += 1;
	}
	
	wait_for_child(data.ids, data.i, data.temp_fd);
}

void	ft_pipe(t_vars *vars)
{
	t_norm	data;

	data.contex.fd_in = STDIN_FILENO;
	data.contex.fd_out = STDOUT_FILENO;
	data.size = get_len(vars->command);
	data.ids = malloc(sizeof(int) * data.size);
	data.i = 0;
	if (data.size != 1)
		loop_through_nodes(vars, data);
	else
	{
		exec_node(vars, vars->command, data.contex);
	}
}