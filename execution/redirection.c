/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:50:01 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/08 13:08:39 by ytouate          ###   ########.fr       */
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
		if (!check_built_in_commands(vars, command))
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
	{
		if (!check_built_in_commands(vars, command))
			ft_execute(command, vars, contex);
	}
}

void	redirect_input(t_vars *vars, t_command *command)
{
	t_contex	contex;
	contex.herdoc_fildes = -1;
	contex = open_files(command);
	if (contex.fd_out == -1 || contex.fd_in == -1)
		return ;
	else if (command->flags[0] != NULL)
		if (!check_built_in_commands(vars, command))
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



int	ft_heredoc(t_vars *vars, t_command *command, t_contex contex)
{
	int		temp_stdin;
	char	*line;
	int		out_file;

	out_file = 0;
	temp_stdin = open("/tmp/temp", O_RDWR | O_TRUNC | O_CREAT, 0777);
	while (true)
	{
		line = readline(">");
		if (line == NULL || ft_strcmp(line, command->herdoc->first_token->value) == 0)
			break ;
		ft_putendl_fd(line, temp_stdin);
	}
	contex = open_files(command);
	if (contex.fd_in == -1 || contex.fd_out == -1)
		return (INT_MIN);
	else if (contex.fd_out != STDOUT_FILENO)
		out_file = -1;
	contex.fd_out = open("/tmp/temp_out_file", O_RDWR | O_TRUNC | O_CREAT, 0777);
	close(temp_stdin);
	temp_stdin = open("/tmp/temp", O_RDONLY);
	contex.fd_in = dup(temp_stdin);
	contex.fd_out = dup(contex.fd_out);
	if (!check_built_in_commands(vars, command))
		if (!check_redirection(vars, command))
			ft_execute(command, vars, contex);
	unlink("tmp/temp");
	if (out_file != -1 && contex.fd_out != STDOUT_FILENO)
		return (contex.fd_out);
	else
		return (-1);
}

bool heredoc_outside_pipe(t_vars *vars, t_command *command)
{
	char *line;
	t_contex contex;
	contex.fd_in = open("/tmp/temp", O_RDWR | O_TRUNC | O_CREAT, 0777);
	if (command->herdoc->first_token == NULL)
		return (false);
	while (true)
	{
		line = readline(">");
		if (line == NULL  || ft_strcmp(line, command->herdoc->first_token->value) == 0)
			break ;
		ft_putendl_fd(line, contex.fd_in);
	}
	close(contex.fd_in);
	contex.fd_in = open("/tmp/temp", O_RDONLY);
	contex.fd_in = dup(contex.fd_in);
	contex.fd_out = STDOUT_FILENO;
	ft_execute(command, vars, contex);
	wait(NULL);
	//if (!check_redirection(vars, command))
	//	if (!check_built_in_commands(vars, command))
	unlink("/tmp/temp");
	return (true);
}