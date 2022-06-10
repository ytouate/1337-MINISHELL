/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execSimpleCommand.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:39:51 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/10 10:01:15 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

char	*get_path(t_list *env_list, char *cmd)
{
	char	*path;
	char	**command_path;
	int		i;

	path = ft_get_env_val(env_list, "PATH");
	if (path == NULL || cmd == NULL)
		return (NULL);
	i = 0;
	command_path = ft_split(path, ':');
	while (command_path[i])
	{
		command_path[i] = ft_strjoin(command_path[i], "/");
		command_path[i] = ft_strjoin(command_path[i], cmd);
		if (access(command_path[i], F_OK) == 0)
			return (command_path[i]);
		i++;
	}
	return (NULL);
}

void	ft_execute(t_command *command, t_vars *vars, t_contex contex)
{
	char	*command_path;

	if (command->flags[0] == NULL)
		return ;
	command_path = get_path(vars->env_list, command->flags[0]);
	if (command->flags[0][0] == '/' || command->flags[0][0] == '.')
		check_cmd(command, vars, contex);
	else if (command_path == NULL)
	{
		set_exit_code(127);
		ft_putstr_fd(command->flags[0], STDERR_FILENO);
		ft_putendl_fd("command not found", STDERR_FILENO);
	}
	else
	{
		if (command->flags[0][0])
		{
			if (fork() == 0)
			{
				dup2(contex.fd_in, STDIN_FILENO);
				dup2(contex.fd_out, STDOUT_FILENO);
				execve(command_path, command->flags, vars->env);
				perror(command->flags[0]);
				exit(0);
			}
			
			wait(NULL);
		}
		else
		{
			ft_putstr_fd(command->flags[0], STDERR_FILENO);
			ft_putendl_fd("command not found", STDERR_FILENO);
		}
	}
	
}

void	run_excutable(t_command *command, t_vars *vars, t_contex contex)
{
	int	status;

	if (access(command->flags[0], F_OK | X_OK) == 0)
	{
		if (fork() == 0)
		{
			if (access(command->flags[0], F_OK | X_OK) == 0)
			{
				dup2(contex.fd_out, STDOUT_FILENO);
				dup2(contex.fd_in, STDIN_FILENO);
				execve(command->flags[0], command->flags, vars->env);
			}
			exit(EXIT_SUCCESS);
		}
		wait(&status);
		if (WIFEXITED(status))
			set_exit_code(WEXITSTATUS(status));
	}
	else
	{
		set_exit_code(126);
		perror(command->flags[0]);
	}
}

void	check_cmd(t_command *command, t_vars *vars, t_contex contex)
{
	int fd[2];

	if (command->flags[0][0] == '/')
	{
		if (access(command->flags[0], F_OK) == 0)
		{
			pipe(fd);
			if (fork() == 0)
			{
				dup2(contex.fd_out, STDOUT_FILENO);
				dup2(contex.fd_in, STDIN_FILENO);
				execve(command->flags[0], command->flags, vars->env);
				perror("execve");
				exit(EXIT_SUCCESS);
			}
		}
		else
		{
			set_exit_code(127);
			perror(command->flags[0]);
		}
	}
	else
		run_excutable(command, vars, contex);
}
