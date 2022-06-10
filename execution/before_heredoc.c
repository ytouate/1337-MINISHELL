/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   before_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 10:37:27 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/10 15:11:13 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

bool	heredoc_exist(t_vars *vars)
{
	while (vars->command)
	{
		if (vars->command->herdoc->first_token != NULL)
			return (true);
		vars ->command = vars->command->next_command;
	}
	return (false);
}

void	exec_first_command_before_heredoc(t_vars *vars, t_norm data)
{
	close(data.fd[0]);
	dup2(data.fd[1], STDOUT_FILENO);
	exec_node(vars, vars->command, data.contex);
}

void	exec_last_command_before_heredoc(t_vars *vars, t_norm data)
{
	if (vars->command->redi->first_token == NULL && vars->command->next_command)
		dup2(data.fd[1], STDOUT_FILENO);
	else
		close(data.fd[1]);
	close(data.fd[0]);
	dup2(data.temp_fd, STDIN_FILENO);
	exec_node(vars, vars->command, data.contex);
}

void	exec_other_command_before_heredoc(t_vars *vars, t_norm data)
{
	close(data.fd[0]);
	dup2(data.fd[1], STDOUT_FILENO);
	dup2(data.temp_fd, STDIN_FILENO);
	exec_node(vars, vars->command, data.contex);
}

int	count_commands_before_heredoc(t_command *command)
{
	int	count;

	count = 0;
	while (command && command->herdoc->first_token == NULL )
	{
		command = command->next_command;
		count += 1;
	}
	return (count);
}

void	check_before_heredoc_commands(t_vars *vars, t_norm data, int i)
{
	if (i == 0)
		exec_first_command_before_heredoc(vars, data);
	else if (i == data.size - 1)
		exec_last_command_before_heredoc(vars, data);
	else
		exec_other_command_before_heredoc(vars, data);
}

void	exec_commands_before_heredoc(t_vars *vars)
{
	int		i;
	t_norm	data;

	i = 0;
	data.size = count_commands_before_heredoc(vars->head->first_c);
	data.ids = malloc(sizeof(int) * data.size);
	vars->command = vars->head->first_c;
	while (i < data.size && vars->command)
	{
		data.contex.fd_in = STDIN_FILENO;
		data.contex.fd_out = STDOUT_FILENO;
		pipe(data.fd);
		data.id = fork();
		if (data.id == 0)
		{
			check_before_heredoc_commands(vars, data, i);
			exit(0);
		}
		data.ids[i++] = data.id;
		vars->command = vars->command->next_command;
		data.temp_fd = dup(data.fd[0]);
		close(data.fd[0]);
		close(data.fd[1]);
	}
	wait_for_child(data.ids, i, data.temp_fd);
}
