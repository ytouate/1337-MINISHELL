/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execSimpleCommand.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:39:51 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/05 21:46:41 by ytouate          ###   ########.fr       */
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

int id = 0;
void func(int sig)
{
	if (sig == SIGINT)
	{
		kill(id, SIGKILL);
		ft_putstr_fd("\n", STDOUT_FILENO);
		rl_on_new_line();
		rl_replace_line("", 0);
		set_exit_code(130);
	}
}
void	ft_execute(t_commande *command, t_vars *vars, t_contex contex)
{
	char	*command_path;
	int		status;
	signal(SIGINT, func);
	if (command->flags[0] == NULL)
		return ;
	command_path = get_path(vars->env_list, command->flags[0]);
	if (command->flags[0][0] == '/' || command->flags[0][0] == '.')
		check_cmd(command, vars, contex);
	else if (command_path == NULL)
	{
		set_exit_code(127);
		printf("%s: command not found\n", command->flags[0]);
	}
	else
	{
		if (command->flags[0][0])
		{
			id = fork();
			if (id == 0)
			{
				dup2(contex.fd_out, STDOUT_FILENO);
				dup2(contex.fd_in, STDIN_FILENO);
				execve(command_path, command->flags, vars->env);
				perror(NULL);
				exit(EXIT_SUCCESS);
			}
			wait(&status);
			if (WIFEXITED(status))
				set_exit_code(WEXITSTATUS(status));
		}
		else
			printf("%s: command not found\n", command->flags[0]);
	}
}

void	run_excutable(t_commande *command, t_vars *vars, t_contex contex)
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

void	check_cmd(t_commande *command, t_vars *vars, t_contex contex)
{
	int fd[2];
	int temp = 0;
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
				temp = 126;
				write(fd[1], &temp, sizeof(int));
				perror("execve");
				exit(EXIT_SUCCESS);
			}
			read(fd[0], &temp, sizeof(int));
			set_exit_code(temp);
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
