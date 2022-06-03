/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 13:35:25 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/03 10:33:13 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"


void exec_first_node(t_vars *vars, int *fd, t_contex contex)
{
	close(fd[0]);
	dup2(fd[1], STDOUT_FILENO);
	exec_node(vars, vars->command, contex);
}

void	exec_last_node(t_vars *vars, int *fd, t_contex contex, int temp_fd)
{
	close (fd[0]);
	close(fd[1]);
	dup2(temp_fd, STDIN_FILENO);
	exec_node(vars, vars->command, contex);
}

void	exec_other_node(t_vars *vars, int *fd, t_contex contex, int temp_fd)
{
	close(fd[0]);
	dup2(fd[1], STDOUT_FILENO);
	dup2(temp_fd, STDIN_FILENO);
	exec_node(vars, vars->command, contex);
}

void	wait_for_child(int *ids, int i, int temp_fd)
{
	close(temp_fd);
	while (ids[--i])
		waitpid(ids[i], 0, 0);
	
}

void	loop_through_nodes(t_vars *vars, t_norm data)
{
	while (vars->command)
	{
		data.contex.fd_in = STDIN_FILENO;
		data.contex.fd_out = STDOUT_FILENO;
		data.contex.herdoc_fildes = -1;
		// if (vars->command->redi->first_token->token == T_HERDOC)
			// data.contex.herdoc_fildes = ft_heredoc(vars, vars->command, data.contex);
		pipe(data.fd);
		data.id = fork();
		if (data.id == 0)
		{
			replace_symbol_by_val(vars->command->flags, vars->env_list);
			if (data.i == 0)
				exec_first_node(vars, data.fd, data.contex);
			else if (data.i == data.size - 1)
				exec_last_node(vars, data.fd, data.contex, data.temp_fd);
			else
				exec_other_node(vars, data.fd, data.contex, data.temp_fd);
			exit(0);
		}
		data.ids[data.i] = data.id;
		data.temp_fd = dup(data.fd[0]);
		close(data.fd[0]);
		close(data.fd[1]);
		vars->command = vars->command->next_comande;
		data.i += 1;
	}
	wait_for_child(data.ids, data.i, data.temp_fd);
	
}

void ft_pipe(t_vars *vars)
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
		exec_node(vars, vars->command, data.contex);
}
