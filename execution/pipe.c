/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 13:35:25 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/11 21:31:24 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void	loop_through_nodes(t_vars *vars, t_norm data)
{
	int	j;

	j = 0;
	data.size -= count_commands_before_heredoc(vars->command);
	data.contex.herdoc_fildes = 1337;
	while (vars->command && vars->command->herdoc->first_token == NULL )
		vars->command = vars->command->next_command;
	while (vars->command)
	{
		data.contex.fd_in = STDIN_FILENO;
		data.contex.fd_out = STDOUT_FILENO;
		if (vars->command->herdoc->first_token != NULL)
		{
			if (vars->command->next_command)
				data.contex.herdoc_fildes = ft_heredoc(vars,
						vars->command, data.contex);
			else
			{
				heredoc_outside_pipe(vars, vars->command);
				wait(NULL);
				break ;
			}
		}
		else
		{
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
			data.ids[j++] = data.id;
			data.contex.herdoc_fildes = 1337;
			data.temp_fd = dup(data.fd[0]);
			close(data.fd[0]);
			close(data.fd[1]);
		}
		data.i += 1;
		vars->command = vars->command->next_command;
	}
	wait_for_child(data.ids, j, data.temp_fd);
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
	}
	else
	{
		if (!heredoc_outside_pipe(vars, vars->command))
			exec_node(vars, vars->command, data.contex);
	}
	free(data.ids);
}
