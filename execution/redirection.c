/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:50:01 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/15 22:05:53 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../MiniShell.h"

void	ft_redirect_output_append_mode(t_command *command, t_vars *vars)
{
	t_contex	contex;

	contex.herdoc_fildes = -1;
	contex = open_files(*command->redi);
	if (contex.fd_in == -1 || contex.fd_out == -1)
	{
		set_exit_code(1);
		return ;
	}
	if (command->flags[0] != NULL)
		if (!check_built_in_commands(vars, command))
			ft_execute(command, vars, contex);
	set_exit_code(EXIT_SUCCESS);
}

void	ft_redirect_output_trunc_mode(t_vars *vars, t_command *command)
{
	t_contex	contex;

	contex.herdoc_fildes = -1;
	contex = open_files(*command->redi);
	if (contex.fd_in == -1 || contex.fd_out == -1)
	{
		set_exit_code(1);
		return ;
	}
	if (command->flags[0] != NULL)
	{
		if (!check_built_in_commands(vars, command))
			ft_execute(command, vars, contex);
	}
	set_exit_code(EXIT_SUCCESS);
}

void	redirect_input(t_vars *vars, t_command *command)
{
	t_contex	contex;

	contex.herdoc_fildes = -1;
	contex = open_files(*command->redi);
	if (contex.fd_out == -1 || contex.fd_in == -1)
	{
		set_exit_code(1);
		return ;
	}
	else if (command->flags[0] != NULL)
		if (!check_built_in_commands(vars, command))
			ft_execute(command, vars, contex);
	set_exit_code(EXIT_SUCCESS);
}

void	exec_herdoc_command(t_command *command, t_vars *vars, t_contex contex)
{
	char	*path;

	if (command->flags[0] != NULL)
	{
		path = get_path(vars->env_list, command->flags[0]);
		if (path)
		{
			if (fork() == 0)
			{
				dup2(contex.herdoc_fildes, STDIN_FILENO);
				dup2(contex.fd_out, STDOUT_FILENO);
				execve(path, command->flags, vars->env);
				perror("execve");
				exit(COMMAND_NOT_FOUND);
			}
		}
		else
		{
			set_exit_code(COMMAND_NOT_FOUND);
			printf("%s: command, not found", command->flags[0]);
		}
	}
	set_exit_code(EXIT_SUCCESS);
}
