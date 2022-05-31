/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:50:01 by ytouate           #+#    #+#             */
/*   Updated: 2022/05/31 16:30:11 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void ft_redirect_output_append_mode(t_commande *command, t_vars *vars)
{
	int fd;
	fd = -1;
	fd = open_output_files(command);
	if (command->flags[0] == NULL)
		return;
	else
	{
        ft_execute(command, vars, fd);
	}
}

void ft_redirect_output_trunc_mode(t_vars *vars, t_commande *command)
{
	int fd;

	fd = open_output_files(command);
	if (fd == -1)
		perror("Error: ");
	else if (command->flags[0] == NULL)
		return;
	else
	{
        ft_execute(command, vars,  fd);
        close(fd);
	}
}
void redirect_input(t_vars *vars, t_commande *command)
{
	int fd;
	char *path;
    
    fd = -1;
	if (command->input->first_token->value == NULL)
		return;
	fd = open_input_files(command);
	if (command->flags[0] != NULL)
	{
		path = get_path(vars->env_list, command->flags[0]);
		if (path == NULL)
			printf("Command Not Found\n");
		else
		{
			if (fork() == 0)
			{
				ft_execute(command, vars, fd);
				exit(EXIT_SUCCESS);
			}
			close(fd);
			wait(NULL);
		}
	}
}
