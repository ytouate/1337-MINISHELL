/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execSimpleCommand.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:39:51 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/18 14:48:38 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../MiniShell.h"

char	*get_path(t_list *env_list, char *cmd)
{
	t_path_vars	vars;

	vars.path = ft_get_env_val(env_list, "PATH");
	if (vars.path == NULL || cmd == NULL)
		return (NULL);
	vars.i = -1;
	vars.command_path = ft_split(vars.path, ':');
	free(vars.path);
	while (vars.command_path[++vars.i])
	{
		vars.temp = vars.command_path[vars.i];
		vars.command_path[vars.i] = ft_strjoin(vars.command_path[vars.i], "/");
		free(vars.temp);
		vars.temp = vars.command_path[vars.i];
		vars.command_path[vars.i] = ft_strjoin(vars.command_path[vars.i], cmd);
		free(vars.temp);
		if (access(vars.command_path[vars.i], F_OK) == 0)
		{
			vars.temp = ft_strdup(vars.command_path[vars.i]);
			free_2d_array(vars.command_path);
			return (vars.temp);
		}
	}
	free_2d_array(vars.command_path);
	return (NULL);
}

void	ft_error(char *arg, char *msg, int exit_code)
{
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putendl_fd(msg, STDERR_FILENO);
	set_exit_code(exit_code);
}

void	exec_command(t_command *command, t_vars *vars,
		t_contex contex, char *command_path)
{
	int	status;

	g_global_vars.pid = fork();
	if (g_global_vars.pid == 0)
	{
		dup2(contex.fd_in, STDIN_FILENO);
		dup2(contex.fd_out, STDOUT_FILENO);
		execve(command_path, command->flags, vars->env);
		perror(command->flags[0]);
		exit(COMMAND_NOT_FOUND);
	}
	wait(&status);
	if (WIFEXITED(status))
		g_global_vars.pid = -1;
	set_exit_code(WEXITSTATUS(status));
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
		set_exit_code(PERMISSION_DENIED);
		perror(command->flags[0]);
	}
}

void	check_cmd(t_command *command, t_vars *vars, t_contex contex)
{
	int	fd[2];
	int	stats;

	if (command->flags[0][0] != '/')
		run_excutable(command, vars, contex);
	else if (access(command->flags[0], F_OK) == 0)
	{
		pipe(fd);
		if (fork() == 0)
		{
			dup2(contex.fd_out, STDOUT_FILENO);
			dup2(contex.fd_in, STDIN_FILENO);
			execve(command->flags[0], command->flags, vars->env);
			perror("execve");
			exit(PERMISSION_DENIED);
		}
		wait(&stats);
		set_exit_code(WEXITSTATUS(stats));
		return ;
	}
	else
	{
		set_exit_code(COMMAND_NOT_FOUND);
		perror(command->flags[0]);
	}
}
