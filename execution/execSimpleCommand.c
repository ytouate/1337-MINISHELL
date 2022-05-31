/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execSimpleCommand.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:39:51 by ytouate           #+#    #+#             */
/*   Updated: 2022/05/31 15:14:01 by ytouate          ###   ########.fr       */
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

void	ft_execute(t_vars *vars, int fd)
{
	char	*command_path;
	int status;
	command_path = get_path(vars->env_list, vars->command->flags[0]);

	if (vars->command->flags[0][0] == '/' || vars->command->flags[0][0] == '.')
		check_cmd(vars, fd);
	else
	{
		if (command_path == NULL)
		{
			ft_putstr_fd(vars->command->flags[0], STDERR_FILENO);
			ft_putstr_fd(": command not found\n", STDERR_FILENO);
			// exit_code = 127;
			return ;
		}
		if (fork() == 0)
		{
            if (fd != -1)
                dup2(fd, STDOUT_FILENO);
			if (execve(command_path, vars->command->flags, vars->env) == -1)
			{
                perror("exeve");
				// exit_code = 126;
			}
			exit(EXIT_SUCCESS);
		}
		wait(&status);
		// set_exit_code(status);
	}
}
int	check_cmd(t_vars *vars, int fd)
{
	int status;
	if (vars->command->flags[0][0] == '/')
	{
		if (fork() == 0)
		{
			if (access(vars->command->flags[0], F_OK) == 0)
			{
                if (fd != -1)
                    dup2(fd, STDOUT_FILENO);
				if (execve(vars->command->flags[0], vars->command->flags, vars->env) == -1)
					perror("execve");
			}
			exit(EXIT_SUCCESS);
		}
		wait(&status);
        if (status == 0)
            set_exit_code(0);
        else
            set_exit_code(126);
		return (0);
	}
	else
	{
		if (fork() == 0)
		{
			if (access(vars->command->flags[0], F_OK | X_OK) == 0)
			{
				if (execve(vars->command->flags[0], vars->command->flags, vars->env) == -1)
				{
					// exit_code = 126;
					perror("execve");
				}
			}
			exit(EXIT_SUCCESS);
		}		
		wait(&status);
		// set_exit_code(status);
	}
	return (0);
}

