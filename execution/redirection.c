/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:50:01 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/18 15:43:42 by ytouate          ###   ########.fr       */
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
		if (!check_built_in_commands(vars, command, contex))
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
		if (!check_built_in_commands(vars, command, contex))
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
		if (!check_built_in_commands(vars, command, contex))
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
			ft_error(command->flags[0],
				": command, not found\n", COMMAND_NOT_FOUND);
	}
	set_exit_code(EXIT_SUCCESS);
}

int	ft_heredoc(t_vars *vars, t_command *command, t_contex contex)
{
	int		temp_stdin;
	int		out_file;

	out_file = 1337;
	temp_stdin = fill_temp_stdin(command);
	contex = open_files(*command->redi);
	if (contex.fd_in == -1 || contex.fd_out == -1)
	{
		set_exit_code(1);
		return (0);
	}
	check_out_files(&out_file, &contex.fd_out);
	check_in_files(&contex.fd_in, &temp_stdin);
	contex.fd_out = dup(contex.fd_out);
	if (!check_built_in_commands(vars, command, contex))
	{
		ft_execute(command, vars, contex);
		wait(NULL);
		close(contex.fd_out);
	}
	unlink("tmp/temp");
	return (heredoc_return(out_file, contex));
}
