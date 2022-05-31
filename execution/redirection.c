/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:50:01 by ytouate           #+#    #+#             */
/*   Updated: 2022/05/31 12:59:20 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void ft_redirect_append_mode(t_vars *vars)
{
	int fd;
	char *path;

	fd = open(vars->command->output->first_token->value, O_CREAT | O_RDWR | O_APPEND, 0644);
	if (vars->command->flags[0] == NULL)
		return;
	else
	{
		if (fork() == 0)
		{
			path = get_path(vars->env_list, vars->command->flags[0]);
			if (path == NULL)
				perror("Error: ");
			else
			{
				dup2(fd, STDOUT_FILENO);
				execve(path, vars->command->flags, vars->env);
			}
			close(fd);
			exit(EXIT_SUCCESS);
		}
		else
			wait(NULL);
	}
}
void ft_redirect_output_trunc_mode(t_vars *vars)
{
	int fd;
	char *path;

	fd = open_output_files(vars->command);
	if (fd == -1)
		perror("Error: ");
	else if (vars->command->flags[0] == NULL)
		return;
	else
	{
		if (fork() == 0)
		{
			path = get_path(vars->env_list, vars->command->flags[0]);
			if (path == NULL)
				perror("Error: ");
			else
			{
				dup2(fd, STDOUT_FILENO);
				execve(path, vars->command->flags, vars->env);
			}
			close(fd);
			exit(EXIT_SUCCESS);
		}
		wait(NULL);
	}
}
void redirect_input(t_vars *vars)
{
	int fd;
	char *path;

	if (vars->command->input->first_token->value == NULL)
		return;
	fd = open_input_files(vars->command);
	if (fd == -1)
		perror(NULL);
	if (vars->command->flags[0] != NULL)
	{
		path = get_path(vars->env_list, vars->command->flags[0]);
		if (path == NULL)
			printf("Command Not Found\n");
		else
		{
			if (fork() == 0)
			{
				dup2(fd, STDIN_FILENO);
				ft_execute(vars);
				exit(EXIT_SUCCESS);
			}
			close(fd);
			wait(NULL);
		}
	}
}
