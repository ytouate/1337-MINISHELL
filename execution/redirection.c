/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:50:01 by ytouate           #+#    #+#             */
/*   Updated: 2022/05/31 14:37:27 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void ft_redirect_output_append_mode(t_vars *vars)
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
char	*join_for_echo(t_list *env_list, char **s, char flag)
{
	int		i;
	char	*result;
	char	**temp;

	if (flag == 'n')
		i = 2;
	else
		i = 1;
	result = "";
	while (s[i])
	{
		if (ft_strcmp(s[i], "~") == 0)
		{
			if (ft_getenv(env_list, "HOME") != NULL)
			{
				temp = ft_split(ft_getenv(env_list, "HOME")->content, '=');
				if (temp[1] != NULL)
					result = ft_strjoin(result, temp[1]);
				free_2d_array(temp);	
			}
		}
		else
			result = ft_strjoin(result, s[i]);
		result = ft_strjoin(result, " ");
		i++;
	}
	result = ft_strtrim(result, " ");
	return (result);
}
