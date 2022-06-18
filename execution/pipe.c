/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 13:35:25 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/18 15:35:38 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../MiniShell.h"

void	exec_after_heredoc(t_vars *vars, t_norm *data, int *j)
{
	pipe(data->fd);
	data->id = fork();
	if (data->id == 0)
	{
		if (data->i == 0)
			exec_first_node(vars, *data);
		else if (data->i == data->size - 1)
			exec_last_node(vars, *data);
		else
			exec_other_node(vars, *data);
		exit(127);
	}
	data->ids[*j++] = data->id;
	data->contex.herdoc_fildes = 1337;
	data->temp_fd = dup(data->fd[0]);
	close(data->fd[0]);
	close(data->fd[1]);
}

void	run_heredoc(int *heredoc_fd, t_vars *vars, t_norm data)
{
	if (vars->command->next_command)
	{
		*heredoc_fd = ft_heredoc(vars,
				vars->command, data.contex);
		wait(NULL);
	}
	else
	{
		heredoc_outside_pipe(vars, vars->command);
	}
}

void	loop_through_nodes(t_vars *vars, t_norm data)
{
	int	j;

	j = 0;
	data.size = get_len(vars->command);
	data.size -= count_commands_before_heredoc(vars->command);
	data.contex.herdoc_fildes = 1337;
	walk_to_heredoc(&vars->command);
	while (vars->command)
	{
		data.contex.fd_in = STDIN_FILENO;
		data.contex.fd_out = STDOUT_FILENO;
		if (vars->command->herdoc->first_token != NULL)
			run_heredoc(&data.contex.herdoc_fildes, vars, data);
		else
		{
			check_commands_order(vars, &data);
			close_pipe(data.fd);
			data.contex.herdoc_fildes = 1337;
		}
		data.ids[j++] = g_global_vars.pid;
		data.i += 1;
		vars->command = vars->command->next_command;
	}
	wait_for_child(data.ids, j, data.temp_fd);
}

bool	check_heredoc(t_command *command)
{
	while (command)
	{
		if (command->herdoc->first_token != NULL)
			return (true);
		command = command->next_command;
	}
	return (command);
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
	{
		loop_through_nodes(vars, data);
		exec_commands_before_heredoc(vars);
		set_exit_code_inside_pipe(vars, vars->head->first_c);
	}
	else
	{
		if (!heredoc_outside_pipe(vars, vars->command))
			exec_node(vars, vars->command, data.contex);
	}
	free(data.ids);
	g_global_vars.pid = -1;
}
