/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:50:01 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/06 14:19:26 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void	ft_redirect_output_append_mode(t_command *command, t_vars *vars)
{
	t_contex	contex;
	contex.herdoc_fildes = -1;
	contex = open_files(command);
	if (contex.fd_in == -1 || contex.fd_out == -1)
		return ;
	if (command->flags[0] != NULL)
		ft_execute(command, vars, contex);
}

void	ft_redirect_output_trunc_mode(t_vars *vars, t_command *command)
{
	t_contex	contex;
	contex.herdoc_fildes = -1;
	contex = open_files(command);
	if (contex.fd_in == -1 || contex.fd_out == -1)
		return ;
	if (command->flags[0] != NULL)
		ft_execute(command, vars, contex);
}

void	redirect_input(t_vars *vars, t_command *command)
{
	t_contex	contex;
	contex.herdoc_fildes = -1;
	contex = open_files(command);
	if (contex.fd_out == -1 || contex.fd_in == -1)
		return ;
	else if (command->flags[0] != NULL)
		ft_execute(command, vars, contex);
}

void	exec_herdoc_command(t_command *command, t_vars *vars, t_contex contex)
{
	char *path;
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
				exit(127);
			}
		}
		else
		{
			set_exit_code(127);
			printf("%s: command, not found", command->flags[0]);
		}
	}
}

void	ft_heredoc(t_vars *vars, t_command *command, t_contex contex)
{
	int temp_file;
	char *line;
	(void)contex;
	(void)vars;
	temp_file = open("out", O_RDWR | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR, 0644);
	if (temp_file == -1)
		return ;
	else
	{
		while (true)
		{
			line = readline(">");
			if (line == NULL || ft_strcmp(line, command->herdoc->first_token->value) == 0)
				break;
			ft_putendl_fd(line, temp_file);
		}
	}
	if (fork() == 0)
	{
		dup2(temp_file, STDIN_FILENO);
		close(temp_file);
		execve(get_path(vars->env_list, command->flags[0]), command->flags, vars->env);
		exit(0);
	}
	else
	{
		wait(NULL);
		close(temp_file);
	}
}