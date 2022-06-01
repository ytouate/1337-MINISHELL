/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 13:35:25 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/01 22:28:34 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void ft_pipe(t_vars *vars)
{
    int	fd[2];
	t_contex contex;
    int temp_fd;
	contex.fd_in = STDIN_FILENO;
	contex.fd_out = STDOUT_FILENO;
	int size;
	size = get_len(vars->command);
    int i;
    int id;
    int *ids = malloc(sizeof(int) * size);
	if (ids == NULL)
		return ;
    i = 0;
	if (size == 1)
	{
		exec_node(vars, vars->command, contex);
	}
	else
	{
		while (vars->command)
		{
			contex.fd_in = STDIN_FILENO;
			contex.fd_out = STDOUT_FILENO;
			pipe(fd);
			id = fork();
			if (id == 0)
			{
				replace_symbol_by_val(vars->command->flags, vars->env_list);
				if (i == 0)
				{
					close(fd[0]);
					if (vars->command->input->first_token != NULL)
					{
						if (vars->command->input->first_token->token == T_HERDOC)
						{
							contex.fd_in = fd[1];
							contex.fd_out = STDOUT_FILENO;
							exec_node(vars, vars->command, contex);
						}
						else
						{
							dup2(fd[1], STDOUT_FILENO);
							exec_node(vars, vars->command, contex);
						}
					}
					else
					{
						dup2(fd[1], STDOUT_FILENO);
						exec_node(vars, vars->command, contex);
					}
					
				}
				else if (i == size - 1)
				{
					close (fd[0]);
					close(fd[1]);
					if (vars->command->input->first_token != NULL)
					{
						if (vars->command->input->first_token->token == T_HERDOC)
						{
							contex.fd_in = fd[1];
							contex.fd_out = STDOUT_FILENO;
							exec_node(vars, vars->command, contex);
						}
					}
					else
					{
						dup2(temp_fd, STDIN_FILENO);
						exec_node(vars, vars->command, contex);
					}
				}
				else
				{
					close(fd[0]);
					if (vars->command->input->first_token != NULL)
					{
						if (vars->command->input->first_token->token == T_HERDOC)
						{
							contex.fd_in = fd[1];
							contex.fd_out = STDOUT_FILENO;
							exec_node(vars, vars->command, contex);
						}
					}
					else
					{
						dup2(fd[1], STDOUT_FILENO);
						dup2(temp_fd, STDIN_FILENO);
						exec_node(vars, vars->command, contex);
					}
				}
				exit(0);
			}
			ids[i] = id;
			temp_fd = dup(fd[0]);
			close(fd[0]);
			close(fd[1]);
			vars->command = vars->command->next_comande;
			i++;
        }
		close(temp_fd);
		while (--i >= 0)
			waitpid(ids[i], 0, 0);
	}
}