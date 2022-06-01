/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execSimpleCommand.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:39:51 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/01 22:26:02 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

char	*get_path(t_list *env_list, char *cmd)
{
	char	*path;
	char	**command_path;
	int		i;

	path = ft_get_env_val(env_list, "PATH");
	if (path == NULL)
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

void	ft_execute(t_commande *command, t_vars *vars, t_contex contex)
{
	char	*command_path;
	int		status;

	command_path = get_path(vars->env_list, command->flags[0]);
	if (command->flags[0][0] == '/' || command->flags[0][0] == '.')
		check_cmd(command, vars, contex);
	else
	{
		if (command_path == NULL)
			printf("%s: command not found\n", command->flags[0]);
		else if (fork() == 0)
		{
			dup2(contex.fd_out, STDOUT_FILENO);
			dup2(contex.fd_in, STDIN_FILENO);
			if (execve(command_path, command->flags, vars->env) == -1)
				perror("exeve");
			exit(EXIT_SUCCESS);
		}
		wait(&status);
	}
}

void	run_excutable(t_commande *command, t_vars *vars, t_contex contex)
{
	int	status;

	if (fork() == 0)
	{
		if (access(command->flags[0], F_OK | X_OK) == 0)
		{
			dup2(contex.fd_out, STDOUT_FILENO);
			dup2(contex.fd_in, STDIN_FILENO);
			if (execve(command->flags[0], command->flags, vars->env) == -1)
				perror("execve");
		}
		else
			perror(command->flags[0]);
		exit(EXIT_SUCCESS);
	}		
	wait(&status);
}

void	check_cmd(t_commande *command, t_vars *vars, t_contex contex)
{
	int	status;

	status = 1;
	if (command->flags[0][0] == '/')
	{
		if (fork() == 0)
		{
			if (access(command->flags[0], F_OK) == 0)
			{
				dup2(contex.fd_out, STDOUT_FILENO);
				dup2(contex.fd_in, STDIN_FILENO);
				if (execve(command->flags[0], command->flags, vars->env) == -1)
					perror("execve");
			}
			else
				perror(command->flags[0]);
			exit(EXIT_SUCCESS);
		}
		wait(&status);
	}
	else
		run_excutable(command, vars, contex);
}
