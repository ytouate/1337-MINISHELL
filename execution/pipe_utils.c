/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 21:15:09 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/18 15:35:29 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../MiniShell.h"

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
	if (data.contex.herdoc_fildes != 1337)
	{
		close(data.contex.herdoc_fildes);
		data.contex.herdoc_fildes = open("/tmp/temp_out_file", O_RDONLY, 0644);
		if (data.contex.herdoc_fildes == 0)
		{
			unlink("/tmp/trash");
			data.contex.herdoc_fildes = open("/tmp/trash", O_RDONLY | O_CREAT);
			dup2(data.contex.herdoc_fildes, STDIN_FILENO);
		}
		else
		{
			dup2(data.contex.herdoc_fildes, STDIN_FILENO);
		}
	}
	else
	{
		dup2(data.temp_fd, STDIN_FILENO);
	}
	exec_node(vars, vars->command, data.contex);
}

void	exec_other_node(t_vars *vars, t_norm data)
{
	close(data.fd[0]);
	if (data.contex.herdoc_fildes != 1337)
	{
		close(data.temp_fd);
		close(data.contex.herdoc_fildes);
		data.contex.herdoc_fildes = open("/tmp/temp_out_file", O_RDONLY);
		dup2(data.contex.herdoc_fildes, STDIN_FILENO);
		dup2(data.fd[1], STDOUT_FILENO);
	}
	else
	{
		dup2(data.fd[1], STDOUT_FILENO);
		dup2(data.temp_fd, STDIN_FILENO);
	}
	exec_node(vars, vars->command, data.contex);
}

void	wait_for_child(int *ids, int i, int temp_fd)
{
	int		status;

	(void)temp_fd;
	while (--i >= 0)
		waitpid(ids[i], &status, 0);
}

void	check_commands_order(t_vars *vars, t_norm *data)
{
	pipe(data->fd);
	g_global_vars.pid = fork();
	if (g_global_vars.pid == 0)
	{
		if (data->i == 0)
			exec_first_node(vars, *data);
		else if (data->i == data->size - 1)
			exec_last_node(vars, *data);
		else
			exec_other_node(vars, *data);
		exit(127);
	}
	data->temp_fd = dup(data->fd[0]);
}
